import {BsFillExclamationTriangleFill} from "react-icons/bs";

import styles from "./NotElevatedPopup.module.scss";

const NotElevatedPopup = ({title, content, buttonText, buttonDisabled, elevateFunc}) => {

    return (
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
            <div className={`${styles['button']} ${buttonDisabled ? styles['disabled'] : ''}`} onClick={() => elevateFunc()}>
                {buttonText}
            </div>
        </div>
    );

};

export default NotElevatedPopup;
