import React from "react";
import {BiTargetLock} from "react-icons/bi";

import styles from "./WindowSwitchPopup.module.scss";
import ElectronUtils from "../util/electron-utils";

class WindowSwitchPopup extends React.PureComponent {

    keyPressHandler = (evt) => {
        if (evt.key === "Escape") {
            ElectronUtils.abortSelection();
            const {closeFunc} = this.props;
            closeFunc();
        }
    };

    componentDidMount() {
        document.addEventListener("keydown", this.keyPressHandler);
    }

    componentWillUnmount() {
        document.removeEventListener("keydown", this.keyPressHandler);
    }

    render() {
        return (
            <div className={styles.modal} onClick={e => e.stopPropagation()}>
                <div className={styles['icon-container']}>
                    <BiTargetLock/>
                </div>
                <div className={styles.content}>
                    <p className={styles['content-header']}>
                        Click on a window to select it
                    </p>
                    <p className={styles['content-footer']}>
                        Press [ESC] to abort
                    </p>
                </div>
            </div>
        );
    }

}

export default WindowSwitchPopup;
