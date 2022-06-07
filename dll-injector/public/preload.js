const {ipcRenderer, contextBridge} = require('electron');

contextBridge.exposeInMainWorld('api', {
    minimize: () => ipcRenderer.invoke('minimize'),
    close: () => ipcRenderer.invoke('close'),
    getProcesses: () => ipcRenderer.send('get-processes'),
    onReceiveProcesses:
        (callback) => ipcRenderer.on('on-receive-processes', (event, data) => callback(data)),
    isElevated: () => ipcRenderer.invoke('is-elevated'),
    restartElevated: () => ipcRenderer.invoke('restart-elevated'), /* TODO check for possible security issues */
    getNextClicked: () => ipcRenderer.send('get-next-clicked'),
    onNextClicked: (callback) => ipcRenderer.on('on-next-clicked', (event, data) => callback(data)),
    abortSelection: () => ipcRenderer.invoke('abort-selection'),
    openDLLDialog: () => ipcRenderer.send('open-dll-dialog'),
    onDLLSelected: (callback) => ipcRenderer.on('on-dll-selected', (event, data) => callback(data))
});
