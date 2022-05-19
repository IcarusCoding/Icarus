import {FaCopyright} from "react-icons/fa";

import styles from "./Footer.module.scss"

const Footer = () => {

    return (
        <div className={styles.footer}>
            <div className={styles["footer-content"]}>
                <FaCopyright className={styles.icon}/>
                <div>
                    {new Date().getFullYear()} IcarusCoding
                </div>
            </div>
        </div>
    );

};

export default Footer;
