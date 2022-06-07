const {app, BrowserWindow, ipcMain, dialog} = require('electron')
const isDev = require("electron-is-dev");
const path = require("path");

const injector = require("icarus_injector");

const elevated = injector.isElevated();

const size = {
    width: 800,
    height: 800
}

function setupHandlers(window) {
    ipcMain.handle('close', (e) => window.close());
    ipcMain.handle('minimize', (e) => window.minimize());
    ipcMain.on('get-processes', async (e) => {
        if (!elevated) {
            return;
        }
        const array = injector.retrieveProcesses();
        e.sender.send('on-receive-processes', array);
    });
    ipcMain.handle('is-elevated', (e) => e.returnValue = elevated);
    ipcMain.handle('restart-elevated', (e) => {
        if (injector.restartElevated() === 0) {
            injector.exitApplication();
            e.returnValue = true;
        } else {
            e.returnValue = false;
        }
    });
    ipcMain.on('get-next-clicked', async (e) => {
        if(!elevated) {
            return;
        }
        injector.getNextClicked((result) => {
            if(result === 4294967295) {
                return;
            }
            e.sender.send('on-next-clicked', result);
        });
    });
    ipcMain.handle('abort-selection', (e) => injector.abortSelection());
    ipcMain.on('open-dll-dialog', async (e) => {
        dialog.showOpenDialog(window, {properties: ['openFile'], filters: [{name: "DLL files", extensions: ["dll"]}]}).then(resp => {
            if(!resp.canceled) {
                e.sender.send("on-dll-selected", resp.filePaths[0]);
            }
        });
    });
}

function initWindow() {
    const window = new BrowserWindow({
        width: size.width,
        height: size.height,
        resizable: isDev,
        webPreferences: {
            preload: isDev ? path.join(app.getAppPath(), './public/preload.js') : path.join(app.getAppPath(), './build/preload.js'),
            contextIsolation: true
        },
        show: false
    });
    setupHandlers(window);
    window.once("ready-to-show", () => window.show());
    if (!isDev) {
        window.removeMenu();
    } else {
        window.webContents.openDevTools();
    }
    window.loadURL(isDev ? 'http://localhost:3000' : `file://${path.join(__dirname, "../build/index.html")}`);
}

app.whenReady().then(async () => initWindow());
