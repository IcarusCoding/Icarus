const {
    app,
    BrowserWindow
} = require('electron');

const log = require("electron-log");

const injector = require("icarus_injector");

log.transports.file.level = 'info';
log.transports.file.resolvePath = () => __dirname + '/latest.log';

const size = {
    width: 960,
    height: 540
}

function initWindow() {
    log.info(`Initializing window with size ${size.width}x${size.height}`);
    const window = new BrowserWindow({
        width: size.width,
        height: size.height,
        show: false
    });
    window.once("ready-to-show", () => window.show());
    window.removeMenu();
    window.loadFile('html/index.html').then(() => {
        log.info('Window initialized');
        console.log(injector.test(100))
    });
}

app.whenReady().then(async () => {
    initWindow();
});