#!/usr/bin/env bash
set -e

# Usage:
#   ./setup_bluetooth.sh <BT_MAC_ADDRESS> [channel]
# Example:
#   ./setup_bluetooth.sh 98:D3:31:F9:12:34 1
#
# This will bind /dev/rfcomm0 to the HC-05/HC-06 device.

if [ -z "$1" ]; then
  echo "Usage: $0 <BT_MAC_ADDRESS> [channel]"
  exit 1
fi

MAC="$1"
CHANNEL="${2:-1}"

echo "Binding /dev/rfcomm0 to $MAC channel $CHANNEL ..."
sudo rfcomm release 0 || true
sudo rfcomm bind 0 "$MAC" "$CHANNEL"
echo "Done. Device: /dev/rfcomm0"


