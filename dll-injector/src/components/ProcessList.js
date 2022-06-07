import {useEffect, useState} from "react";
import {BiTargetLock} from "react-icons/bi";
import {HiOutlineRefresh} from "react-icons/hi";
import {MdErrorOutline} from "react-icons/md";

import ProcessListEntry from "./ProcessListEntry";
import ElectronUtils from "../util/electron-utils";

import styles from "./ProcessList.module.scss";

const ProcessList = ({selectFunc, switchTo}) => {

    const [SearchInput, SetSearchInput] = useState("");
    const [Processes, SetProcesses] = useState([]);

    const inputChanged = evt => SetSearchInput(evt.target.value);

    const processRequest = () => ElectronUtils.getProcesses();

    useEffect(() => {
        ElectronUtils.onReceiveProcesses(processes => SetProcesses(processes.map(process => JSON.parse(process))));
        processRequest();
    }, []);

    return (
      <div className={styles['content-container']}>
          <div className={styles.searchbar}>
              <button type="submit" onClick={() => selectFunc()}>
                  <BiTargetLock className={styles.icon} style={{fontSize: "20px"}}/>
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
                      Processes.filter(item => item.name.toUpperCase().startsWith(SearchInput.toUpperCase())).sort((a, b) => {
                          const aName = a["name"].toUpperCase();
                          const bName = b["name"].toUpperCase();
                          if(aName > bName) {
                              return 1;
                          } else if(aName < bName) {
                              return -1;
                          }
                          return 0;
                      }).map((item, index) => {
                          return (
                              <ProcessListEntry key={index} name={item.name} exePath={item.exePath} pid={item.pid} arch={item.arch} icon={item.icon} onClickFunc={() => switchTo("dll")}/>
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
