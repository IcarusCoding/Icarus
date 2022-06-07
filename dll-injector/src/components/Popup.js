import React from "react";

import styles from "./Popup.module.scss";

class Popup extends React.PureComponent {

    render() {
        return (
            <div className={styles['popup-background']}>
                {React.cloneElement(React.Children.only(this.props.children), this.props.popupState)}
            </div>
        );
    }

}

export default Popup;
