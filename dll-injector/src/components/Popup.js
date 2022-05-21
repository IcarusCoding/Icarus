import {BsFillExclamationTriangleFill} from "react-icons/bs"

import styles from "./Popup.module.scss";

const Popup = ({title, content, buttonText, buttonDisabled, closeFunc, buttonFunc}) => {

    return (
        <div className={styles['popup-background']}>
            <div className={styles.modal} onClick={e => e.stopPropagation()}>
                <div className={styles['icon-container']}>
                    <span/>
                    <BsFillExclamationTriangleFill/>
                </div>
                <div className={styles['content-container']}>
                    <span className={styles.title}>
                        {title}
                    </span>
                    <p className={styles.content}>
                        {content}
                    </p>
                </div>
                <div className={`${styles['button']} ${buttonDisabled ? styles['disabled'] : ''}`} onClick={() => buttonFunc()}>
                    {buttonText}
                </div>
            </div>
        </div>
    );

}

export default Popup;
