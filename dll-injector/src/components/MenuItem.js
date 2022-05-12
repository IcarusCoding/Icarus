import styles from "./Header.module.scss";

const MenuItem = ({name, icon, active, click}) => {
    return (
        <li className={active ? styles.active : null} onClick={click}>
            <a>
                <span className={styles.icon}>{icon}</span>
                <span className={styles.text}>{name}</span>
            </a>
        </li>
    );
};

export default MenuItem