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

    static receiveProcesses(func) {
        if (ElectronUtils.runsInElectron()) {
            window.api.receiveProcesses(data => func(data));
        }
    }
}
