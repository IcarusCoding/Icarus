import React from "react";
import {GiProcessor} from 'react-icons/gi';
import {VscLibrary} from 'react-icons/vsc';
import {FiSettings} from 'react-icons/fi';
import {FaSyringe} from 'react-icons/fa';
import {BsPencil} from 'react-icons/bs';

import MenuItem from "./MenuItem";

import styles from "./Header.module.scss";

class Header extends React.Component {

    constructor(props) {
        super(props);
        this.state = {
            activeIndex: 1
        };
    }

    selectStep(index) {
        this.setState({
           activeIndex: index
        });
    }

    render() {
        return (
            <nav className={styles.navigator}>
                <ul>
                    <MenuItem name="Process" icon={<GiProcessor/>} active={this.state.activeIndex === 1} click={() => this.props.switchTo("process")}/>
                    <MenuItem name="DLL" icon={<VscLibrary/>} active={this.state.activeIndex === 2} click={() => this.props.switchTo("dll")}/>
                    <MenuItem name="Method" icon={<BsPencil/>} active={this.state.activeIndex === 3} click={() => this.props.switchTo("method")}/>
                    <MenuItem name="Settings" icon={<FiSettings/>} active={this.state.activeIndex === 4} click={() => this.props.switchTo("settings")}/>
                    <MenuItem name="Inject" icon={<FaSyringe/>} active={this.state.activeIndex === 5} click={() => this.props.switchTo("inject")}/>
                    <div className={styles.selector}/>
                </ul>
            </nav>
        );
    }

}

export default Header;
