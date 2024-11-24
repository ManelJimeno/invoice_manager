import QtQuick 2.10

Item {
    id: baseFont

    readonly property string basePath: "./ttf/" + fontName + "/" + fontName
    readonly property string bold: boldFont.name
    readonly property string extraLight: extraLightFont.name
    property string fontName
    property string fontType
    readonly property string light: lightFont.name
    readonly property string medium: mediumFont.name
    readonly property string semiBold: semiBoldFont.name

    FontLoader {
        id: boldFont

        source: baseFont.basePath + "-Bold." + baseFont.fontType
    }
    FontLoader {
        id: semiBoldFont

        source: baseFont.basePath + "-SemiBold." + baseFont.fontType
    }
    FontLoader {
        id: mediumFont

        source: baseFont.basePath + "-Medium." + baseFont.fontType
    }
    FontLoader {
        id: lightFont

        source: baseFont.basePath + "-Light." + baseFont.fontType
    }
    FontLoader {
        id: extraLightFont

        source: baseFont.basePath + "-ExtraLight." + baseFont.fontType
    }
}
