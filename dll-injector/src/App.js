import React from "react";

import Header from "./components/Header";
import Footer from "./components/Footer";
import ProcessList from "./components/ProcessList";
import Switcher from "./components/Switcher";
import Injection from "./components/Injection";
import Popup from "./components/Popup";

import "./App.scss";

import ElectronUtils from "./util/electron-utils";
import NotElevatedPopup from "./components/NotElevatedPopup";
import WindowSwitchPopup from "./components/WindowSwitchPopup";
import DllList from "./components/DllList";

class App extends React.Component {

    constructor(props) {
        super(props);
        this.state = {
            popupSelector: 0,
            popupState: {}
        };
    }

    componentDidMount() {
        ElectronUtils.isElevated()
            .then(elevated => elevated ? this.setup() : this.initNotElevatedPopup());
    }

    setup() {
        ElectronUtils.onNextClicked(PID => {
            console.log("RECV -> " + PID);
            if (this.state.popupSelector === 2) {
                this.closePopup();
            }
        })
    }

    initNotElevatedPopup() {
        this.setState({
            popupSelector: 1,
            popupState: {
                title: "Error!",
                content: "Injector does not run with admin privileges.",
                buttonText: "Restart as admin",
                buttonDisabled: false,
                elevateFunc: () => {
                    this.setPopupButtonDisabled(true);
                    ElectronUtils.restartElevated().then(() => this.setPopupButtonDisabled(false));
                }
            }
        });
    }

    initWindowSwitchPopup() {
        this.setState({
            popupSelector: 2,
            popupState: {
                closeFunc: () => this.closePopup()
            }
        });
    }

    closePopup() {
        console.log("CLOSE")
        this.setState({popupSelector: 0});
    }

    handleWindowSelection() {
        this.initWindowSwitchPopup();
        console.log("HANDLE " + JSON.stringify(this.state))
        ElectronUtils.getNextClicked();
    }

    setPopupButtonDisabled(disabled) {
        const popupState = this.state.popupState;
        popupState.buttonDisabled = disabled;
        this.setState({popupState: popupState});
    }

    switchTo(name) {
        this.switcher.switchTo(name);
    }

    selectStep(name) {
        this.header.selectStep(name);
    }

    render() {
        return (
            <div className="root-container">
                <div className="header-container">
                    <Header ref={header => this.header = header} switchTo={this.switchTo.bind(this)}/>
                </div>
                <div className="main-container">
                    <Switcher ref={switcher => this.switcher = switcher} selectStep={this.selectStep.bind(this)}>
                        <ProcessList stepName={"process"} selectFunc={this.handleWindowSelection.bind(this)}/>
                        <DllList stepName={"dll"}/>
                        <Footer stepName={"method"}/>
                        <Footer stepName={"settings"}/>
                        <Injection stepName={"inject"}/>
                    </Switcher>
                </div>
                <div className="footer-container">
                    <Footer/>
                </div>
                {this.state.popupSelector > 0 &&
                    <Popup popupState={this.state.popupState}>
                        {
                            this.state.popupSelector === 1 ?
                                <NotElevatedPopup/> :
                                <WindowSwitchPopup/>
                        }
                    </Popup>
                }
            </div>
        );
    }

}

export default App;
