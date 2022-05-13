"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var ElectronUtils = /** @class */ (function () {
    function ElectronUtils() {
    }
    ElectronUtils.runsInElectron = function () {
        if (typeof window !== 'undefined' && typeof window.process === 'object' && window.process.type === 'renderer') {
            return true;
        }
        if (typeof process !== 'undefined' && typeof process.versions === 'object' && !!process.versions.electron) {
            return true;
        }
        return typeof navigator === 'object' && typeof navigator.userAgent === 'string' && navigator.userAgent.indexOf('Electron') >= 0;
    };
    ElectronUtils.closeElectron = function () {
        if (ElectronUtils.runsInElectron()) {
            var remote = window.require('@electron/remote');
            remote.getCurrentWindow().close();
        }
    };
    return ElectronUtils;
}());
exports.default = ElectronUtils;
