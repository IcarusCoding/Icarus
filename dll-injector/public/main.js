const { app, BrowserWindow } = require('electron')
const isDev = require("electron-is-dev");
const path = require("path");
const injector = require("icarus_injector");

const remoteMain = require("@electron/remote/main");

const size = {
    width: 960,
    height: 540
}

function initWindow() {
    remoteMain.initialize();
    //log.info(`Initializing window with size ${size.width}x${size.height}`);
    const window = new BrowserWindow({
        width: size.width,
        height: size.height,
        resizable: false,
        webPreferences: {
            nodeIntegration: true,
            enableRemoteModule: true,
            contextIsolation: false
        },
        show: false
    });
    remoteMain.enable(window.webContents);
    window.once("ready-to-show", () => window.show());
   // window.removeMenu();
    window.loadURL(isDev ? 'http://localhost:3000' : `file://${path.join(__dirname, "../build/index.html")}`).then(() => {
        //log.info('Window initialized');
        console.log(injector.test(100))
    });
}

app.whenReady().then(async () => {
    initWindow();
});