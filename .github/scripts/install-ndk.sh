#!/usr/bin/env bash
set -euo pipefail

VER="$1"
DEST="$RUNNER_TEMP/android-ndk-$VER"
ZIP="$RUNNER_TEMP/android-ndk-$VER.zip"
URL="https://dl.google.com/android/repository/android-ndk-${VER}-linux.zip"

if [ -d "$DEST" ] && [ -f "$DEST/build/cmake/android.toolchain.cmake" ]; then
  echo "$DEST" > /tmp/ndk-path.txt
  exit 0
fi

echo "Downloading NDK $VER dari $URL"
for i in 1 2 3; do
  if curl -fL --retry 2 --retry-delay 3 -o "$ZIP" "$URL"; then
    break
  fi
  echo "Percobaan $i gagal, ulangi..."
  sleep 5
done

[ -s "$ZIP" ] || { echo "Gagal unduh NDK $VER"; exit 1; }
unzip -q "$ZIP" -d "$RUNNER_TEMP"
[ -f "$DEST/build/cmake/android.toolchain.cmake" ] || { echo "Struktur NDK tidak valid"; exit 1; }
echo "$DEST" > /tmp/ndk-path.txt
echo "NDK siap di $DEST"
