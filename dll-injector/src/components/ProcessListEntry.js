import styles from "./ProcessListEntry.module.scss";

const ProcessListEntry = ({name, exePath, pid, arch, icon}) => {

    return (
        <div className={styles.entry}>
            <div className={styles["entry-icon-container"]}>
                <div className={styles["entry-arch-container"]}>
                    <h5 className={styles["entry-arch"]}>{arch}</h5>
                </div>
                <div className={styles["entry-image-container"]}>
                    <img draggable="false" className={styles.image} src={`data:image/png;base64,${icon}`}/>
                </div>
            </div>
            <div className={styles["entry-text-container"]}>
                <p className={styles["entry-name"]}>
                    {name}
                </p>
                <p className={styles["entry-path"]}>
                    {exePath}
                </p>
            </div>
            <div className={styles["entry-misc-container"]}>
                <div className={styles["entry-arch-container"]}>
                    <h5 className={styles["entry-arch"]}>{arch}</h5>
                </div>
                <div className={styles["entry-pid-container"]}>
                    <h5 className={styles["entry-pid"]}>{pid}</h5>
                </div>
            </div>
        </div>
    );

};

export default ProcessListEntry;
