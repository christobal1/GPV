#!/usr/bin/env bash
set -euo pipefail

APP_NAME="GPV"
ROOT_DIR="$(pwd)"
APP_DIR="$ROOT_DIR/$APP_NAME.app"
CONTENTS="$APP_DIR/Contents"
MACOS="$CONTENTS/MacOS"
RESOURCES="$CONTENTS/Resources"
FRAMEWORKS="$CONTENTS/Frameworks"
BINARY="$ROOT_DIR/$APP_NAME"
BIN_COPY="$RESOURCES/$APP_NAME.bin"
WRAPPER="$MACOS/$APP_NAME"

if [ ! -f "$BINARY" ]; then
  echo "💡 GPV ist nicht gebaut. Baue jetzt..."
  make
fi

if [ ! -x "$BINARY" ]; then
  chmod +x "$BINARY"
fi

rm -rf "$APP_DIR"
mkdir -p "$MACOS" "$RESOURCES" "$FRAMEWORKS"

cp "$BINARY" "$BIN_COPY"
chmod +x "$BIN_COPY"

cp -R details "$RESOURCES/" 
cp -R info_pictures "$RESOURCES/"
cp -R saves "$RESOURCES/"

cat > "$CONTENTS/Info.plist" <<'EOF'
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
  <key>CFBundleName</key>
  <string>GPV</string>
  <key>CFBundleDisplayName</key>
  <string>GPV</string>
  <key>CFBundleIdentifier</key>
  <string>com.christobal1.gpv</string>
  <key>CFBundleVersion</key>
  <string>1.0</string>
  <key>CFBundleExecutable</key>
  <string>GPV</string>
  <key>CFBundlePackageType</key>
  <string>APPL</string>
  <key>LSMinimumSystemVersion</key>
  <string>10.12</string>
</dict>
</plist>
EOF

cat > "$WRAPPER" <<'EOF'
#!/usr/bin/env bash
HERE="$(cd "$(dirname "$0")" && pwd)"
cd "$HERE/../Resources"
exec "$HERE/../Resources/GPV.bin" "$@"
EOF
chmod +x "$WRAPPER"

copy_library() {
  local libpath="$1"
  local libname="$(basename "$libpath")"
  local target="$FRAMEWORKS/$libname"

  if [ -e "$target" ]; then
    return 0
  fi

  echo "📦 Kopiere $libpath"
  cp -L "$libpath" "$target"
  install_name_tool -id "@rpath/$libname" "$target"

  local deps
  deps=$(otool -L "$target" | tail -n +2 | awk '{print $1}')
  for dep in $deps; do
    case "$dep" in
      /usr/lib/*|/System/Library/Frameworks/*|@rpath/*|@loader_path/*)
        continue
        ;;
    esac
    copy_library "$dep"
  done

  local dep
  deps=$(otool -L "$target" | tail -n +2 | awk '{print $1}')
  for dep in $deps; do
    case "$dep" in
      /usr/lib/*|/System/Library/Frameworks/*|@rpath/*|@loader_path/*)
        continue
        ;;
    esac
    local depname="$(basename "$dep")"
    if [ -e "$FRAMEWORKS/$depname" ]; then
      install_name_tool -change "$dep" "@rpath/$depname" "$target"
    fi
  done
}

rewrite_library_references() {
  local target="$1"
  local deps
  deps=$(otool -L "$target" | tail -n +2 | awk '{print $1}')
  for dep in $deps; do
    case "$dep" in
      /usr/lib/*|/System/Library/Frameworks/*|@rpath/*|@loader_path/*)
        continue
        ;;
    esac
    local depname="$(basename "$dep")"
    if [ -e "$FRAMEWORKS/$depname" ]; then
      install_name_tool -change "$dep" "@rpath/$depname" "$target"
    else
      echo "⚠️  Fehlende gebündelte Bibliothek: $dep"
    fi
  done
}

copy_library "$BIN_COPY"
install_name_tool -add_rpath "@executable_path/../Frameworks" "$BIN_COPY" || true
rewrite_library_references "$BIN_COPY"
for libfile in "$FRAMEWORKS"/*.dylib; do
  rewrite_library_references "$libfile"
done

cat <<EOF
✅ Bundle fertig: $APP_DIR
Öffne mit: open "$APP_DIR"
EOF
