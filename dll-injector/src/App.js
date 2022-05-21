import React from "react";

import Header from "./components/Header";
import Footer from "./components/Footer";
import ProcessList from "./components/ProcessList";
import Switcher from "./components/Switcher";
import Injection from "./components/Injection";
import Popup from "./components/Popup";

import "./App.scss";
import ElectronUtils from "./util/electron-utils";

class App extends React.Component {

    constructor(props) {
        super(props);
        this.state = {
            popupOpen: false,
            popupTitle: "",
            popupContent: "",
            popupButtonText: "",
            popupButtonDisabled: false
        };
        this.openPopup = this.openPopup.bind(this);
        this.closePopup = this.closePopup.bind(this);
    }

    componentDidMount() {
        ElectronUtils.isElevated()
            .then(elevated => elevated ? null : this.openPopup("Error!", "Injector does not run with admin privileges.", "Restart as admin"));
    }

    openPopup(title, content, buttonText) {
        this.setState({
            popupOpen: true,
            popupTitle: title,
            popupContent: content,
            popupButtonText: buttonText
        });
    }

    closePopup() {
        this.setState({
            popupOpen: false
        });
    }

    setPopupButtonDisabled(disabled) {
        this.setState({
            popupButtonDisabled: disabled
        });
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
                        <ProcessList stepName={"process"}/>
                        <Footer stepName={"dll"}/>
                        <Footer stepName={"method"}/>
                        <Footer stepName={"settings"}/>
                        <Injection stepName={"inject"}/>
                    </Switcher>
                </div>
                <div className="footer-container">
                    <Footer/>
                </div>
                {this.state.popupOpen && <Popup title={this.state.popupTitle} content={this.state.popupContent} buttonText={this.state.popupButtonText} buttonDisabled={this.state.popupButtonDisabled} buttonFunc={() => {
                    this.setPopupButtonDisabled(true);
                    ElectronUtils.restartElevated().then(success => this.setPopupButtonDisabled(success));
                }} closeFunc={this.closePopup}/>}
            </div>
        );
    }

}

export default App;
