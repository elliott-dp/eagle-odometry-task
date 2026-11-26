#!/bin/bash
# Plays a bag inside the GUI container, for the odometry node and RViz.
# Runs on the Mac (from CLion or a terminal) and drives the container with docker exec.
#
#   tools/play_bag.sh [options] [BAG]
#
#   BAG               a bag folder in the bags folder (default: perception_bag), a recording
#                     made with --record (recordings/NAME or just NAME), or any bag folder
#                     inside this project
#   --list            show the bags you can play, then exit
#   --rate R          play at R times real time. The Debug build needs about 1 s per scan,
#                     so give it 0.05; the RelWithDebInfo build keeps up at 1
#   --from S          start S seconds into the bag (the odometry then starts there)
#   --paused          start paused: SPACE plays, the right arrow plays one message
#   --record NAME     also record everything the node publishes into recordings/NAME,
#                     to replay it later without the node and jump back and forth in it
#   --wait-for-node   first wait for /lidar_odometry (CLion may still be building it)
#
# Keys, in the terminal running this script: SPACE pause/resume, right arrow one message
# while paused, up/down arrow rate +-10%. tools/bag_ctl.sh does the same from anywhere,
# and also jumps back and forth in a recording.
set -u
export PATH="/usr/local/bin:/opt/homebrew/bin:$PATH"   # docker, when CLion was started from the Dock
export DOCKER_CLI_HINTS=false                          # no "What's next" adverts after docker exec

CONTAINER="${EAGLE_CONTAINER:-eagle-gui}"
PKG_DIR="$(cd "$(dirname "$0")/.." && pwd)"   # same path on the Mac and in the container
REPO_DIR="$(cd "$PKG_DIR/../../.." && pwd)"
RECORDINGS="$REPO_DIR/recordings"
QOS="$PKG_DIR/config/bag_qos.yaml"

usage() { sed -n '2,21p' "$0" | sed 's/^# \{0,1\}//'; exit "${1:-0}"; }

BAG_ARG=perception_bag RATE=1 FROM="" PAUSED="" RECORD="" WAIT="" LIST=""
while [ $# -gt 0 ]; do
  case "$1" in
    --list) LIST=1 ;;
    --rate) RATE="${2:?--rate needs a number}"; shift ;;
    --from) FROM="${2:?--from needs seconds}"; shift ;;
    --paused) PAUSED=1 ;;
    --record) RECORD="${2:?--record needs a name}"; shift ;;
    --wait-for-node) WAIT=1 ;;
    -h|--help) usage ;;
    -*) echo "Unknown option $1"; usage 1 ;;
    *) BAG_ARG="$1" ;;
  esac
  shift
done

docker start "$CONTAINER" >/dev/null || { echo "Cannot start the container $CONTAINER"; exit 1; }

# The Mac folder mounted at /data/bags in the container (read-only).
BAGS_HOST="$(docker inspect -f '{{range .Mounts}}{{if eq .Destination "/data/bags"}}{{.Source}}{{end}}{{end}}' "$CONTAINER")"

# ros2 with the workspace sourced, so the player and the recorder know the ProcessingStats type.
ros() {
  docker exec "$CONTAINER" bash -lc "source /opt/ros/humble/setup.bash && \
    { [ -f '$REPO_DIR/install/setup.bash' ] && source '$REPO_DIR/install/setup.bash'; } ; $1"
}

if [ -n "$LIST" ]; then
  echo "In the bags folder ($BAGS_HOST):"
  docker exec "$CONTAINER" bash -c \
    'for d in /data/bags/*/; do [ -f "$d/metadata.yaml" ] && echo "  $(basename "$d")"; done'
  echo "Recordings ($RECORDINGS):"
  for d in "$RECORDINGS"/*/; do [ -f "$d/metadata.yaml" ] && echo "  recordings/$(basename "$d")"; done
  exit 0
fi

# Turns BAG into the path the container sees.
resolve_bag() {
  local p
  for p in "$1" "$REPO_DIR/$1" "$RECORDINGS/$1"; do
    [ -f "$p/metadata.yaml" ] || continue
    p="$(cd "$p" && pwd)"
    case "$p" in
      "$REPO_DIR"/*) echo "$p"; return 0 ;;                                 # mounted at the same path
      "$BAGS_HOST"/*) echo "/data/bags/${p#"$BAGS_HOST"/}"; return 0 ;;
    esac
    echo "$p is not visible in $CONTAINER: move it into the project or into $BAGS_HOST" >&2
    return 1
  done
  if docker exec "$CONTAINER" test -f "/data/bags/$1/metadata.yaml"; then
    echo "/data/bags/$1"; return 0
  fi
  echo "No bag called $1. These are the ones you can play:" >&2
  "$0" --list >&2
  return 1
}
BAG="$(resolve_bag "$BAG_ARG")" || exit 1

node_running() { ros "ros2 node list 2>/dev/null" | grep -qx "/lidar_odometry"; }

# A recording already holds the odometry, so the node must not run and publish a second one.
if docker exec "$CONTAINER" grep -q "name: /odometry$" "$BAG/metadata.yaml"; then
  REVIEW=1
  [ -n "$RECORD" ] && { echo "$BAG_ARG is already a recording: --record does not apply"; exit 1; }
  if node_running; then
    echo "$BAG_ARG is a recording: it already holds the odometry. Stop the odometry node first,"
    echo "or it publishes a second, different estimate on top of it."
    exit 1
  fi
else
  REVIEW=""
fi

# Played straight from the external drive, through Docker's file sharing, the player cannot read
# the bag fast enough: nothing comes out for seconds, then a burst bigger than the node's input
# queue, and the scans it drops make the odometry lose track. So the bag is copied once onto the
# container's own disk and played from there. A copy whose metadata no longer matches is redone.
BAG="$(docker exec "$CONTAINER" bash -c '
  src="$1"; dst="$HOME/.cache/bags$src"
  if ! cmp -s "$src/metadata.yaml" "$dst/metadata.yaml" 2>/dev/null; then
    echo "Copying $(basename "$src") ($(du -sh "$src" | cut -f1)) into the container, once..." >&2
    rm -rf "$dst" "$dst.part" && mkdir -p "$(dirname "$dst")" &&
      cp -r "$src" "$dst.part" && mv "$dst.part" "$dst" || exit 1
  fi
  echo "$dst"' _ "$BAG")" || { echo "Could not copy the bag into $CONTAINER"; exit 1; }

stop_player() { docker exec "$CONTAINER" pkill -INT -f "ros2 bag play" >/dev/null 2>&1; }
stop_recorder() { docker exec "$CONTAINER" pkill -INT -f "ros2 bag record" >/dev/null 2>&1; }

# A second player would feed two copies of the drive into the odometry.
stop_player && { echo "Stopped a bag that was already playing in $CONTAINER"; sleep 1; }

if [ -n "$WAIT" ]; then
  echo "Waiting for /lidar_odometry to start (CLion may still be building it)..."
  until node_running; do sleep 2; done
  sleep 1   # let its subscription match before the first scan goes out
fi

if [ -n "$RECORD" ]; then
  OUT="$RECORDINGS/$RECORD"
  [ -e "$OUT" ] && { echo "$OUT already exists: pick another name or delete it"; exit 1; }
  mkdir -p "$RECORDINGS"
  # Recordings are hundreds of MB: keep them out of git.
  [ -f "$RECORDINGS/.gitignore" ] || echo "*" > "$RECORDINGS/.gitignore"
  stop_recorder
  # Everything but /clock, which the player makes again when the recording is played. With
  # --use-sim-time it is stored on the bag's clock, like the stamps in the messages.
  docker exec -d "$CONTAINER" bash -lc "source /opt/ros/humble/setup.bash && \
    { [ -f '$REPO_DIR/install/setup.bash' ] && source '$REPO_DIR/install/setup.bash'; } ; \
    exec ros2 bag record --use-sim-time -a -x '/clock|/rosout|/parameter_events' -o '$OUT' > /tmp/record.log 2>&1"
  sleep 2   # let the recorder subscribe before the first scan goes out
  echo "Recording into $OUT"
fi

finish() {
  stop_player
  [ -n "$RECORD" ] || return
  # The node may still be working through scans the player sent long ago: keep recording
  # until it has published nothing for 5 s. -k: ros2 sometimes ignores the TERM from timeout.
  echo "Bag finished. Recording until the node stops publishing..."
  while ros "timeout -k 2 5 ros2 topic echo --once /odometry" >/dev/null 2>&1; do :; done
  stop_recorder; sleep 2
  echo "Recorded $OUT. Replay it with: tools/play_bag.sh recordings/$RECORD (stop the node first)"
}

# Stopping this script from CLion must stop the player in the container too.
trap finish EXIT
trap 'exit 130' INT TERM HUP

ARGS="--clock --rate $RATE --qos-profile-overrides-path '$QOS'"
[ -n "$FROM" ] && ARGS="$ARGS --start-offset $FROM"
[ -n "$PAUSED" ] && ARGS="$ARGS --start-paused"
[ -n "$REVIEW" ] && echo "Replaying the recording $BAG_ARG: jump around with tools/bag_ctl.sh back/forward/seek"

TTY=-i; [ -t 0 ] && TTY=-it
# No -l: when the bag restarts, the car jumps back to the start and the odometry cannot follow.
docker exec $TTY "$CONTAINER" bash -lc "source /opt/ros/humble/setup.bash && \
  { [ -f '$REPO_DIR/install/setup.bash' ] && source '$REPO_DIR/install/setup.bash'; } ; \
  exec ros2 bag play '$BAG' $ARGS"
