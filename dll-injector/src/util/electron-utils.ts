declare global {
    interface Window {
        api? : any
    }
}
export default class ElectronUtils {

    static runsInElectron() {
        if (typeof window !== 'undefined' && typeof window.process === 'object' && window.process.type === 'renderer') {
            return true;
        }
        if (typeof process !== 'undefined' && typeof process.versions === 'object' && !!process.versions.electron) {
            return true;
        }
        return typeof navigator === 'object' && typeof navigator.userAgent === 'string' && navigator.userAgent.indexOf('Electron') >= 0;
    }

    static closeElectron() {
        if (ElectronUtils.runsInElectron()) {
            window.api.close();
        }
    }

    static minimizeElectron() {
        if (ElectronUtils.runsInElectron()) {
            window.api.minimize();
        }
    }

    static getProcesses() {
        if (ElectronUtils.runsInElectron()) {
            return window.api.getProcesses();
        }
    }

    static onReceiveProcesses(func) {
        if (ElectronUtils.runsInElectron()) {
            window.api.onReceiveProcesses(data => func(data));
        }
    }

    static isElevated() {
        if(ElectronUtils.runsInElectron()) {
            return window.api.isElevated();
        }
        return Promise.resolve(true); /* TODO Refactor whole class and force electron use */
    }

    static restartElevated() {
        if(ElectronUtils.runsInElectron()) {
            return window.api.restartElevated();
        }
        return Promise.resolve(true); /* TODO Refactor whole class and force electron use */
    }

    static getNextClicked() {
        if (ElectronUtils.runsInElectron()) {
            return window.api.getNextClicked();
        }
    }

    static onNextClicked(func) {
        if (ElectronUtils.runsInElectron()) {
            window.api.onNextClicked(data => func(data));
        }
    }

    static abortSelection() {
        if(ElectronUtils.runsInElectron()) {
            window.api.abortSelection();
        }
    }

    static openDLLDialog() {
        if (ElectronUtils.runsInElectron()) {
            window.api.openDLLDialog();
        }
    }

    static onDLLSelected(func) {
        if (ElectronUtils.runsInElectron()) {
            window.api.onDLLSelected(data => func(data));
        }
    }

}
