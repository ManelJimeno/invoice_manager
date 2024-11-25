pragma Singleton

import QtQuick 2.9

Item {
    readonly property string awesome: awesome.name
    readonly property string backgroundColor: currentTheme[0]
    property var currentTheme: themes.dark
    readonly property BaseFont draftingMono: draftingMono
    readonly property string font_family: outfit.medium
    readonly property string mainAppColor: currentTheme[1]
    readonly property string mainTextColor: currentTheme[2]
    readonly property BaseFont outfit: outfit
    readonly property string popupBackgroundColor: currentTheme[3]
    readonly property string popupTextColor: currentTheme[4]
    property alias themes: themes

    QtObject {
        id: themes

        readonly property var dark: ["#394454", "#6fda9c", "#f0f0f0", "#b44", "#ffffff"]
        // https://colorhunt.co/palette/f3c892fff1bda3da8d146356
        readonly property var light: ["#F3C892", "#FFF1BD", "#A3DA8D", "#146356", "#146356"]
    }
    BaseFont {
        id: draftingMono

        fontName: "DraftingMono"
        fontType: "otf"
    }
    BaseFont {
        id: outfit

        fontName: "Outfit"
        fontType: "ttf"
    }
    FontLoader {
        id: awesome

        source: "./ttf/Font Awesome 6 Free-Solid-900.otf"
    }
}
