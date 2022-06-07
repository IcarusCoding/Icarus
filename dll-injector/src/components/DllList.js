import {useEffect, useState} from "react";

import DllListEntry from "./DllListEntry";
import ElectronUtils from "../util/electron-utils";

import styles from "./DllList.module.scss";

const DllList = () => {

    const [dlls, setDlls] = useState([]);

    useEffect(() => ElectronUtils.onDLLSelected(file => setDlls([...dlls, {
        name: file.split('\\').pop(),
        path: file,
        arch: 'x99'
    }])));

    return (
        <div className={styles['content-container']}>
            <div className={styles.addbar}>
                <div onClick={() => ElectronUtils.openDLLDialog()}>ADD DLL</div>
            </div>
            <div className={styles.container}>
                {
                    dlls.map((item, index) => {
                        return (
                            <DllListEntry key={index} name={item.name} path={item.path} arch={item.arch}/>
                        );
                    })
                }
            </div>
        </div>
    )

};

export default DllList;