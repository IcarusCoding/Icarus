const { app, BrowserWindow } = require('electron')
const isDev = require("electron-is-dev");
const path = require("path");
const injector = require("icarus_injector");

const size = {
    width: 960,
    height: 540
}

function initWindow() {
    //log.info(`Initializing window with size ${size.width}x${size.height}`);
    const window = new BrowserWindow({
        width: size.width,
        height: size.height,
        show: false
    });
    window.once("ready-to-show", () => window.show());
    window.removeMenu();
    window.loadURL(isDev ? 'http://localhost:3000' : `file://${path.join(__dirname, "../build/index.html")}`).then(() => {
        //log.info('Window initialized');
        console.log(injector.test(100))
    });
}

app.whenReady().then(async () => {
    initWindow();
});