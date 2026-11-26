#!/bin/bash
# Controls the bag that tools/play_bag.sh is playing, from any terminal on the Mac.
#
#   tools/bag_ctl.sh pause | resume | toggle
#   tools/bag_ctl.sh next [N]      while paused: play the next N messages (default 1)
#   tools/bag_ctl.sh rate R        play at R times real time
#   tools/bag_ctl.sh back S        jump S seconds back      } only while replaying a recording:
#   tools/bag_ctl.sh forward S     jump S seconds forward   } the live node cannot follow a jump
#   tools/bag_ctl.sh seek S        jump to S seconds from the start of the bag
#   tools/bag_ctl.sh where         how far into the bag the player is
set -u
export PATH="/usr/local/bin:/opt/homebrew/bin:$PATH"
export DOCKER_CLI_HINTS=false

CONTAINER="${EAGLE_CONTAINER:-eagle-gui}"
REPO_DIR="$(cd "$(dirname "$0")/../../../.." && pwd)"

usage() { sed -n '2,10p' "$0" | sed 's/^# \{0,1\}//'; exit "${1:-0}"; }

ros() {
  docker exec "$CONTAINER" bash -lc "source /opt/ros/humble/setup.bash && \
    { [ -f '$REPO_DIR/install/setup.bash' ] && source '$REPO_DIR/install/setup.bash'; } ; $1"
}

# Calls a service of the player and prints only its answer, such as "success=True".
call() {
  local request="${3:-}"
  [ -n "$request" ] || request='{}'
  local answer
  answer="$(ros "timeout -k 2 10 ros2 service call /rosbag2_player/$1 rosbag2_interfaces/srv/$2 '$request'" 2>&1)" ||
    { echo "The player did not answer (has the bag finished?)"; exit 1; }
  answer="$(echo "$answer" | grep -A1 '^response:' | tail -1 | sed -E 's/.*_Response\((.*)\)/\1/')"
  echo "${answer:-ok}"
}

# The bag being played: the folder in the player's command line.
playing_bag() {
  docker exec "$CONTAINER" pgrep -af "ros2 bag play" |
    awk '{ for (i = 1; i < NF; i++) if ($i == "play") { print $(i + 1); exit } }'
}

case "${1:-}" in -h|--help|"") usage ;; esac

BAG="$(playing_bag)"
[ -n "$BAG" ] || { echo "No bag is playing in $CONTAINER: start one with tools/play_bag.sh"; exit 1; }

# Bag times are in nanoseconds since the epoch.
bag_start_ns() {
  docker exec "$CONTAINER" awk '/nanoseconds_since_epoch/ { print $2; exit }' "$BAG/metadata.yaml"
}
now_ns() {
  local ns
  ns="$(ros "timeout -k 2 5 ros2 topic echo --once /clock" |
    awk '/sec:/ { if (!s) s = $2; else n = $2 } END { if (s) printf "%d%09d\n", s, n }')"
  [ -n "$ns" ] || { echo "No /clock from the player (has the bag finished?)" >&2; exit 1; }
  echo "$ns"
}
seconds() { awk -v ns="$1" -v t0="$2" 'BEGIN { printf "%.1f", (ns - t0) / 1e9 }'; }

seek_to_ns() {
  if ! docker exec "$CONTAINER" grep -q "name: /odometry$" "$BAG/metadata.yaml" &&
     ros "ros2 node list 2>/dev/null" | grep -qx "/lidar_odometry"; then
    echo "The odometry node is running on this bag. After a jump it would get scans from"
    echo "somewhere else and lose track. Record a run (play_bag.sh --record NAME), stop the"
    echo "node, replay the recording (play_bag.sh recordings/NAME) and jump around in that."
    exit 1
  fi
  local ns=$1
  call seek Seek "{time: {sec: $((ns / 1000000000)), nanosec: $((ns % 1000000000))}}"
}

case "${1:-}" in
  pause) call pause Pause ;;
  resume) call resume Resume ;;
  toggle) call toggle_paused TogglePaused ;;
  next) call burst Burst "{num_messages: ${2:-1}}" ;;
  rate) call set_rate SetRate "{rate: ${2:?rate needs a number}}" ;;
  where)
    NOW="$(now_ns)" || exit 1
    echo "$(seconds "$NOW" "$(bag_start_ns)") s into $BAG" ;;
  back|forward)
    S="${2:?$1 needs seconds}"
    NOW="$(now_ns)" || exit 1
    DELTA="$(awk -v s="$S" 'BEGIN { printf "%d", s * 1e9 }')"
    [ "$1" = back ] && DELTA=$((-DELTA))
    TARGET=$((NOW + DELTA)); T0="$(bag_start_ns)"
    [ "$TARGET" -lt "$T0" ] && TARGET=$T0
    echo "$(seconds "$NOW" "$T0") s -> $(seconds "$TARGET" "$T0") s"
    seek_to_ns "$TARGET" ;;
  seek)
    T0="$(bag_start_ns)"
    seek_to_ns $((T0 + $(awk -v s="${2:?seek needs seconds}" 'BEGIN { printf "%d", s * 1e9 }'))) ;;
  *) echo "Unknown command $1"; usage 1 ;;
esac
