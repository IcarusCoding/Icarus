import {useEffect, useState} from "react";
import {FaSearch} from "react-icons/fa";
import {HiOutlineRefresh} from "react-icons/hi";
import {MdErrorOutline} from "react-icons/md";

import ProcessListEntry from "./ProcessListEntry";
import ElectronUtils from "../util/electron-utils";

import styles from "./ProcessList.module.scss";

const ProcessList = () => {

    const [SearchInput, SetSearchInput] = useState("");
    const [Processes, SetProcesses] = useState([]);

    const inputChanged = evt => SetSearchInput(evt.target.value);

    const processRequest = () => ElectronUtils.getProcesses();

    useEffect(() => {
        processRequest();
        ElectronUtils.receiveProcesses(processes => SetProcesses(processes.map(process => JSON.parse(process))));
    }, []);

    return (
      <div className={styles['content-container']}>
          <div className={styles.searchbar}>
              <button type="submit">
                  <FaSearch className={styles.icon} style={{fontSize: "16px"}}/>
              </button>
              <div className={styles['v-spacer']}/>
              <input type="text" placeholder="Search for a process..." onChange={inputChanged} maxLength={40}/>
              <div className={styles['v-spacer']}/>
              <button type="submit" onClick={processRequest}>
                  <HiOutlineRefresh className={styles.icon} style={{fontSize: "20px"}}/>
              </button>
          </div>
          <div className={styles.container}>
              {
                  Processes.filter(item => item.name.toUpperCase().startsWith(SearchInput.toUpperCase())).length === 0 ?
                  <div className={styles["error-container"]}>
                      <div className={styles["icon-container"]}>
                          <MdErrorOutline/>
                      </div>
                      <div className={styles["text-container"]}>
                          {Processes.length === 0 ? "There is no process available." : `There is no process which starts with "${SearchInput}".`}
                      </div>
                  </div> :
                      Processes.filter(item => item.name.toUpperCase().startsWith(SearchInput.toUpperCase())).map((item, index) => {
                          return (
                              <ProcessListEntry key={index} name={item.name} exePath={item.exePath} pid={item.pid} arch="x64" icon={item.icon}/>
                          );
                      })
              }
          </div>
      </div>
    );
};

ProcessList.defaultProps = {
  items: [{name: "Placeholder.exe", exePath: "path/to/exe", icon: "", arch: "x64"}]
};

export default ProcessList;
