const {app, BrowserWindow, ipcMain} = require('electron')
const isDev = require("electron-is-dev");
const path = require("path");

const injector = require("icarus_injector");

const size = {
    width: 800,
    height: 800
}

function setupHandlers(window) {
    ipcMain.handle('close', (e) => window.close());
    ipcMain.handle('minimize', (e) => window.minimize());
    ipcMain.on('get-processes', async (e) => {
        const array = injector.retrieveProcesses();
        e.sender.send('receive-processes', array);
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
    window.once("ready-to-show", () => window.show());
    if (!isDev) {
        window.removeMenu();
    } else {
        window.webContents.openDevTools();
    }
    window.loadURL(isDev ? 'http://localhost:3000' : `file://${path.join(__dirname, "../build/index.html")}`)
        .then(() => setupHandlers(window));
}

app.whenReady().then(async () => initWindow());
