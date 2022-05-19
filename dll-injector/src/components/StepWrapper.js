import "./Switcher.css";

const StepWrapper = ({children, isActive, transitions}) => {
    return (
        <div className={`step animated ${transitions} ${isActive ? 'active' : ''}`.trim()}>
            {children}
        </div>
    );
};

export default StepWrapper;
