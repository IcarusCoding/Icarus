import React from "react";

import Header from "./components/Header";
import Footer from "./components/Footer";
import ProcessList from "./components/ProcessList";
import Switcher from "./components/Switcher";
import Injection from "./components/Injection";

import "./App.scss";

class App extends React.Component {

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
            </div>
        );
    }

}

export default App;
