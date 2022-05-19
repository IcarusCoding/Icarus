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
            window.api.close();
        }
    };
    ElectronUtils.minimizeElectron = function () {
        if (ElectronUtils.runsInElectron()) {
            window.api.minimize();
        }
    };
    ElectronUtils.getProcesses = function () {
        if (ElectronUtils.runsInElectron()) {
            return window.api.getProcesses();
        }
    };
    ElectronUtils.receiveProcesses = function (func) {
        if (ElectronUtils.runsInElectron()) {
            window.api.receiveProcesses(function (data) { return func(data); });
        }
    };
    return ElectronUtils;
}());
exports.default = ElectronUtils;
