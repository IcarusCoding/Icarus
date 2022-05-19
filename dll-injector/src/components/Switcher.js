import React from 'react';

import StepWrapper from "./StepWrapper";

import "./Animation.css";
import "./Switcher.css";

class Switcher extends React.PureComponent {

    constructor(props) {
        super(props);
        this.state = this.init();
    }

    init() {
        const state = {
            active: 0,
            classes: {},
            steps: {}
        };
        const children = this.getSteps();
        children.forEach((child, index) => {
            state.steps[index] = child.props && child.props.stepName;
            state.steps[state.steps[index]] = index;
        });
        return state;
    }

    setActive = next => {
        const active = this.state.active;
        if (active === next) {
            return;
        }
        const {classes} = this.state;
        if (active < next) {
            classes[active] = 'exitLeft';
            classes[next] = 'enterRight';
        } else {
            classes[active] = 'exitRight';
            classes[next] = 'enterLeft';
        }
        this.setState({
            active: next,
            classes
        }, () => this.props.selectStep(next + 1));
    };

    getSteps = () => React.Children.toArray(this.props.children);

    switchTo = name => this.setActive(this.state.steps[name]);

    render() {
        const props = {
            switchTo: this.switchTo
        };
        const {classes} = this.state;
        const childrenProps = React.Children.map(this.getSteps(), (child, index) => {
            props.transitions = classes[index];
            props.isActive = (this.state.active === index);
            return (
                <StepWrapper {...props}>
                    {React.cloneElement(child, props)}
                </StepWrapper>
            );
        });
        return (
            <div className="step-wrapper-container">
                {childrenProps}
            </div>
        );
    }
}

export default Switcher;
