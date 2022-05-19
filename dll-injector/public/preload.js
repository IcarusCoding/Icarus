const { ipcRenderer, contextBridge } = require('electron');

contextBridge.exposeInMainWorld('api', {
    minimize: () => ipcRenderer.invoke('minimize'),
    close: () => ipcRenderer.invoke('close'),
    getProcesses: () => ipcRenderer.send('get-processes'),
    receiveProcesses:
        (callback) => ipcRenderer.on('receive-processes', (event, data) => callback(data))
});