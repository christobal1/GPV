#!/usr/bin/env bash

echo "🔍 Erkenne Betriebssystem..."

OS="$(uname -s)"

install_debian() {
    echo "➡ Debian/Ubuntu/Mint erkannt."
    echo "📦 Installiere GTK3..."
    sudo apt update
    sudo apt install -y libgtk-3-0
}

install_fedora() {
    echo "➡ Fedora / RHEL / CentOS erkannt."
    echo "📦 Installiere GTK3..."
    sudo dnf install -y gtk3
}

install_arch() {
    echo "➡ Arch Linux erkannt."
    echo "📦 Installiere GTK3..."
    sudo pacman -Sy --noconfirm gtk3
}

install_macos() {
    echo "➡ macOS erkannt."
    if ! command -v brew >/dev/null 2>&1; then
        echo "❌ Homebrew ist nicht installiert!"
        echo "Installiere es mit:"
        echo '/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"'
        exit 1
    fi
    echo "📦 Installiere GTK3..."
    brew install gtk+3
}

case "$OS" in
    Linux)
        if [ -f "/etc/debian_version" ]; then
            install_debian
        elif [ -f "/etc/fedora-release" ]; then
            install_fedora
        elif [ -f "/etc/arch-release" ]; then
            install_arch
        else
            echo "❌ Unbekannte Linux-Distribution!"
            echo "Bitte GTK3 manuell installieren."
            exit 1
        fi
        ;;
    Darwin)
        install_macos
        ;;
    *)
        echo "❌ Unsupported OS: $OS"
        exit 1
        ;;
esac

echo "✅ GTK Installation abgeschlossen!"