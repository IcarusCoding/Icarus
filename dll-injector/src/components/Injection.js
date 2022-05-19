import styles from "./Injection.module.scss";

const Injection = () => {

    return (
        <div className={styles['content-container']}>
            <div className={styles['temp-container']}>
                <div className={styles['glow-button']}>
                    <span>
                        Inject
                    </span>
                </div>
            </div>
        </div>
    );

};

export default Injection;
