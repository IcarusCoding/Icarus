import {AiOutlineDelete} from "react-icons/ai";

import styles from "./DllListEntry.module.scss";

const DllListEntry = ({name, path, arch}) => {

    return (
        <div className={styles.entry}>
            <div>
                {name}
            </div>
            <div>
                {path}
            </div>
            <div>
                {arch}
            </div>
            <button type="submit" onClick={() => {}}>
                <AiOutlineDelete/>
            </button>
        </div>
    );

};

export default DllListEntry;