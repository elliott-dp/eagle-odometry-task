#!/bin/bash
# Plays the perception bag inside the GUI container, for the odometry node to consume.
# Runs on the Mac (from CLion or a terminal) and drives the container with docker exec.
#   tools/play_bag.sh                  play now
#   tools/play_bag.sh --wait-for-node  first wait for /lidar_odometry (CLion may still be building it)
set -u
export PATH="/usr/local/bin:/opt/homebrew/bin:$PATH"   # docker, when CLion was started from the Dock
export DOCKER_CLI_HINTS=false                          # no "What's next" adverts after docker exec

CONTAINER="${EAGLE_CONTAINER:-eagle-gui}"
BAG="${EAGLE_BAG:-/data/bags/perception_bag}"
PKG_DIR="$(cd "$(dirname "$0")/.." && pwd)"   # same path on the Mac and in the container
QOS="$PKG_DIR/config/bag_qos.yaml"

ros() { docker exec "$CONTAINER" bash -lc "source /opt/ros/humble/setup.bash && $1"; }
stop_player() { docker exec "$CONTAINER" pkill -INT -f "ros2 bag play" >/dev/null 2>&1; }

docker start "$CONTAINER" >/dev/null || { echo "Cannot start the container $CONTAINER"; exit 1; }

# A second player would feed two copies of the drive into the odometry.
stop_player && { echo "Stopped a bag that was already playing in $CONTAINER"; sleep 1; }

if [ "${1:-}" = "--wait-for-node" ]; then
  echo "Waiting for /lidar_odometry to start (CLion may still be building it)..."
  until ros "ros2 node list 2>/dev/null" | grep -qx "/lidar_odometry"; do sleep 2; done
  sleep 1   # let its subscription match before the first scan goes out
fi

# Stopping this script from CLion must stop the player in the container too.
trap stop_player EXIT
trap 'exit 130' INT TERM HUP

TTY=-i; [ -t 0 ] && TTY=-it
# No -l: when the bag restarts, the car jumps back to the start and the odometry cannot follow.
docker exec $TTY "$CONTAINER" bash -lc \
  "source /opt/ros/humble/setup.bash && exec ros2 bag play '$BAG' --clock --qos-profile-overrides-path '$QOS'"
