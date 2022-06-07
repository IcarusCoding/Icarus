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
    ElectronUtils.onReceiveProcesses = function (func) {
        if (ElectronUtils.runsInElectron()) {
            window.api.onReceiveProcesses(function (data) { return func(data); });
        }
    };
    ElectronUtils.isElevated = function () {
        if (ElectronUtils.runsInElectron()) {
            return window.api.isElevated();
        }
        return Promise.resolve(true); /* TODO Refactor whole class and force electron use */
    };
    ElectronUtils.restartElevated = function () {
        if (ElectronUtils.runsInElectron()) {
            return window.api.restartElevated();
        }
        return Promise.resolve(true); /* TODO Refactor whole class and force electron use */
    };
    ElectronUtils.getNextClicked = function () {
        if (ElectronUtils.runsInElectron()) {
            return window.api.getNextClicked();
        }
    };
    ElectronUtils.onNextClicked = function (func) {
        if (ElectronUtils.runsInElectron()) {
            window.api.onNextClicked(function (data) { return func(data); });
        }
    };
    ElectronUtils.abortSelection = function () {
        if (ElectronUtils.runsInElectron()) {
            window.api.abortSelection();
        }
    };
    ElectronUtils.openDLLDialog = function () {
        if (ElectronUtils.runsInElectron()) {
            window.api.openDLLDialog();
        }
    };
    ElectronUtils.onDLLSelected = function (func) {
        if (ElectronUtils.runsInElectron()) {
            window.api.onDLLSelected(function (data) { return func(data); });
        }
    };
    return ElectronUtils;
}());
exports.default = ElectronUtils;
