import React, { Component } from 'react';
import mqtt from 'mqtt';
import _ from 'lodash';
import Potentiometer from './Potentiometer_pb';
import './App.css';

// 1   -> set Brightness
// 100 -> getState req
// 101 -> getState ack

const MQTT_TOPIC_OUT = "esp/protobuf"
const MQTT_TOPIC_IN  = "esp/protobuf/set"

class App extends Component {

  constructor(props) {
    super(props);

    const client = mqtt.connect("ws://broker.mqttdashboard.com:8000/mqtt")

    client.on('connect',  this.onConnect)
    client.on('message',  this.onMessageReceived)

    this.state = {
      connected: false,
      brightness: undefined,
      messages: [],
      client,
    };
  }

  componentWillUnmount() {
    this.state.client.end()
  }

  componentDidMount() {
    this.setBrightness = this.setBrightness.bind(this)
    this.onBrightnessChange = this.onBrightnessChange.bind(this)
  }

  onConnect = () => {
    const { client } = this.state;

    const Brightness = new Potentiometer.Brightness()
    Brightness.setId(100); // 100 -> getState
    const bytes = Brightness.serializeBinary();
    
    // console.log(require('util').inspect(bytes, { depth: null }));
    

    client.subscribe(MQTT_TOPIC_IN)
    client.publish(MQTT_TOPIC_IN, bytes, { qos: 1 }, this.setState({ connected: true }))
    client.subscribe(MQTT_TOPIC_OUT);
  };

  onConnectionLost = responseObject => {
    if (responseObject.errorCode !== 0) {
      console.log(`connection lost: ${responseObject.errorMessage}`);
    }
  };

  onMessageReceived = (topic, message, packet) => {    
    const from = topic.toString()
    const brightness = Potentiometer.Brightness.deserializeBinary(new Uint8Array(message)).toObject()
    
    console.log(require('util').inspect(brightness, { depth: null }));

    // switch (brightness.id) {
    //   case 101:
    //     this.setState({ brightness: brightness.level })
    //     break;
    //   case 1:
    //     console.log(brightness);
    //     break;

    //   default:
    //     return;
    // }

    switch (from) {
      case MQTT_TOPIC_OUT:
        if (brightness.id === 101) this.setState({ brightness: brightness.level })
        break;
      case MQTT_TOPIC_IN:
        console.log(brightness);
        break;
      default:
        return;
    }
  };

  setBrightness(level) {
    const { client } = this.state;

    if (level !== null && typeof level === 'number') {
      const Brightness = new Potentiometer.Brightness()
      Brightness.setId(1); // 1 -> set Brightness
      Brightness.setLevel(level);
      const bytes = Brightness.serializeBinary();
      client.publish(MQTT_TOPIC_IN, bytes)
    }
  }

  onBrightnessChange(event) {
    const level = Math.floor(event.target.value)
    this.setState({brightness: level});
    this.setBrightness(level)
  }

  render() {
    const { connected, brightness } = this.state
    return (
      <div className="App">
        <div className="SliderBox">
          { connected && brightness !== undefined ? (
            <div>
              <h1 className="Level">{brightness}</h1>
              <input className="Slider" type="range" min="0" max="255" value={brightness} onChange={this.onBrightnessChange} />
            </div>
          ) : <h1 className="Status">Connecting ... </h1> }
        </div>
      </div>
    );
  }
}

export default App;
