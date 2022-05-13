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
            const remote = window.require('@electron/remote');
            remote.getCurrentWindow().close()
        }
    }


}