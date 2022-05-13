import {GiProcessor} from 'react-icons/gi';
import {VscLibrary} from 'react-icons/vsc';
import {FiSettings} from 'react-icons/fi';
import {FaSyringe} from 'react-icons/fa';
import {BsPencil} from 'react-icons/bs';
import {FaTimes} from 'react-icons/fa';

import {useState} from "react";

import styles from './Header.module.scss';

import MenuItem from "./MenuItem";

import ElectronUtils from "../util/electron-utils";

const Header = () => {
    const [activeIndex, setActiveIndex] = useState(0);

    return (
        <nav className={styles.navigator}>
            <ul>
                <MenuItem name="Process" icon={<GiProcessor/>} active={activeIndex === 0} click={() => setActiveIndex(0)}/>
                <MenuItem name="DLL" icon={<VscLibrary/>} active={activeIndex === 1} click={() => setActiveIndex(1)}/>
                <MenuItem name="Method" icon={<BsPencil/>} active={activeIndex === 2} click={() => setActiveIndex(2)}/>
                <MenuItem name="Settings" icon={<FiSettings/>} active={activeIndex === 3} click={() => setActiveIndex(3)}/>
                <MenuItem name="Inject" icon={<FaSyringe/>} active={activeIndex === 4} click={() => setActiveIndex(4)}/>
                <div className={styles.selector}/>
                <div className={styles.close} onClick={() => ElectronUtils.closeElectron()}>
                    <a>
                        <span><FaTimes/></span>
                    </a>
                </div>
            </ul>
        </nav>
    )
};

export default Header;






