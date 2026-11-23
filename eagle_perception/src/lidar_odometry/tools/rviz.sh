#!/bin/bash
# Opens RViz inside the GUI container and shows it in a window on the Mac.
# Runs on the Mac (from CLion or a terminal) and drives the container with docker exec.
#
# XQuartz cannot give a container the OpenGL that RViz needs: it refuses indirect GLX, and
# its visuals do not match Mesa's software renderer. So RViz draws into Xephyr, a small X
# server inside the container, with Mesa's software OpenGL 4.5. Xephyr then shows the
# finished pixels in an ordinary XQuartz window, and XQuartz never sees any OpenGL.
set -u
export PATH="/usr/local/bin:/opt/homebrew/bin:$PATH"   # docker, when CLion was started from the Dock
export DOCKER_CLI_HINTS=false                          # no "What's next" adverts after docker exec

CONTAINER="${EAGLE_CONTAINER:-eagle-gui}"
PKG_DIR="$(cd "$(dirname "$0")/.." && pwd)"   # same path on the Mac and in the container
CONFIG="$PKG_DIR/rviz/odometry.rviz"
SCREEN=1400x820   # the Xephyr window; Window Geometry in the .rviz makes RViz fill it exactly

# Xephyr reaches XQuartz over TCP, which XQuartz blocks unless this box is ticked.
if [ "$(defaults read org.xquartz.X11 nolisten_tcp 2>/dev/null)" != "0" ]; then
  echo "XQuartz refuses connections from the container."
  echo "Open XQuartz > Settings > Security, tick 'Allow connections from network clients',"
  echo "quit XQuartz with Cmd-Q, then run this again."
  exit 1
fi

open -g -a XQuartz   # starts it in the background if it is not running
# X forgets xhost grants when its last client disconnects. While XQuartz is still starting,
# xhost can be that last client, so wait for its window manager to connect first.
for _ in $(seq 20); do pgrep -x quartz-wm >/dev/null && break; sleep 0.5; done
# Docker Desktop forwards the container's connection from localhost.
DISPLAY=:0 /opt/X11/bin/xhost +localhost >/dev/null || { echo "XQuartz is not answering on :0"; exit 1; }

docker start "$CONTAINER" >/dev/null || { echo "Cannot start the container $CONTAINER"; exit 1; }

# Images built before xserver-xephyr was added to the Dockerfile lack it: install it once here.
if ! docker exec "$CONTAINER" bash -c 'command -v Xephyr' >/dev/null; then
  echo "Installing Xephyr in $CONTAINER (once per container, needs internet)..."
  docker exec -u root "$CONTAINER" bash -c 'apt-get update -qq &&
    DEBIAN_FRONTEND=noninteractive apt-get install -y -qq --no-install-recommends xserver-xephyr >/dev/null' \
    || { echo "Could not install xserver-xephyr in $CONTAINER"; exit 1; }
fi

stop_viewer() {
  docker exec "$CONTAINER" pkill -f "rviz2 -d $CONFIG" >/dev/null 2>&1
  docker exec "$CONTAINER" pkill -x Xephyr >/dev/null 2>&1
}
stop_viewer   # one left over from an earlier run
sleep 0.5

# Start Xephyr as display :1 in the container. Its window goes to XQuartz (the container's
# DISPLAY). Remove stale lock files first, left behind if the container stopped with Xephyr running.
docker exec -u root "$CONTAINER" bash -c 'mkdir -p -m 1777 /tmp/.X11-unix && rm -f /tmp/.X1-lock /tmp/.X11-unix/X1'
docker exec -d -e DISPLAY=host.docker.internal:0 "$CONTAINER" bash -c \
  "exec Xephyr :1 -screen $SCREEN -title 'RViz (lidar_odometry)' -ac -br -noreset -nolisten tcp -no-host-grab > /tmp/xephyr.log 2>&1"
for _ in $(seq 20); do docker exec "$CONTAINER" test -S /tmp/.X11-unix/X1 && break; sleep 0.5; done
if ! docker exec "$CONTAINER" test -S /tmp/.X11-unix/X1; then
  echo "Xephyr did not start:"; docker exec "$CONTAINER" cat /tmp/xephyr.log; exit 1
fi

trap stop_viewer EXIT   # closing RViz also closes its window, and the other way round
trap 'exit 130' INT TERM HUP

# In the background plus wait: bash runs a trap only once its foreground command returns, and
# docker exec returns only when RViz does. Stopping the script must not have to wait for that.
docker exec -e DISPLAY=:1 -e LIBGL_ALWAYS_SOFTWARE=1 "$CONTAINER" bash -lc \
  "source /opt/ros/humble/setup.bash && exec rviz2 -d '$CONFIG' --ros-args -p use_sim_time:=true" &
wait $!
