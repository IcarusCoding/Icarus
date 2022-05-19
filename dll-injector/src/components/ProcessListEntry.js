import styles from "./ProcessListEntry.module.scss";

const ProcessListEntry = ({name, exePath, pid, arch, icon}) => {

    return (
        <div className={styles.entry}>
            <div className={styles["image-container"]}>
                <img draggable="false" className={styles.image} src={`data:image/png;base64,${icon}`}/>
            </div>
            <div className={styles["card-container"]}>
                <div className={styles["card-header"]}>
                    <h5>{name}</h5>
                    <h5>{pid}</h5>
                    <h5>{arch}</h5>
                </div>
                <span className={styles["card-text"]}>{exePath}</span>
            </div>
        </div>
    );

};

export default ProcessListEntry;
