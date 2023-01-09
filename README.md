<h2 align="center">
    Agriculture Node
</h2>
<p align="center">
  <a href="#about">About</a> •
  <a href="#schematic">Schematic</a> •
  <a href="#protocol / algorithm">Protocol</a> •
  <a href="#firmware docs">Docs</a> •
  <a href="#license">License</a>
</p>

<p align="center">
  <a href="https://dpiclimate.github.io/ag-node">
    <img src="https://github.com/DPIClimate/ag-node/actions/workflows/main.yml/badge.svg"
         alt="documentation-badge">
  </a>
</p>

<img title="" src="figures/system.png" alt="" style="display: flex; align-items: center; justify-content: space-around;">

## About

The agriculture node (ag-node) is an automatic multi-weigh-station for measuring the live-weight of sheep  in remote locations. The station can be situated against a trough for which sheep weights will be recorded as they access feed or water. An in-built LoRaWAN radio sends data (containing live-weight information) back to a local gateway and onto the end user. Additional interfaces are provided, including; SDI-12, I<sub>2</sub>C and OneWire to allow for various sensors to be connected to the device (such as trough level sensors, temperature monitors etc.).

## Schematic

The devices schematic can be found at: [schematic.pdf](https://github.com/DPIclimate/ag-node/blob/master/figures/schematic.pdf)

Basic components are summarised in the below image. Where A is the analog-to-digital converter, B is the LoRaWAN communication infrastructure and C is the peripheral sensor connectors (I<sub>2</sub>C, OneWire and SDI-12).

<div align="middle"> 
	<img src="https://raw.githubusercontent.com/DPIclimate/ag-node/master/figures/schematic.png" style="width: 70%;">
</div>

## Protocol / Algorithm

As the device operates autonomously (sheep are weighed in an unsupervised manner) an algorithm to capture the maximum stable weight (and an error representation) is provided.

The protocol is explained below and a visual representation is provided.

1. A weight is captured (triggered by a sheep stepping on a scale) consisting of a time-series of weights while the sheep is on the platform

2. This time-series data is sliced into groups of 5 values

3. The slope between the first and last value is calculated (where values < 0.1) are discarded as they represent unstable weights

4. The Root Mean Square Error (RMSE) of the fitted line is calculated

5. The average of the 5 points are taken

6. An evaluation of the new weight is made against previous slices (see figure below under the Method `Evaluate` for an equation that represents this)

<div align="middle"> 
	<img src="https://raw.githubusercontent.com/DPIclimate/ag-node/master/figures/weigh-algorithm.png" style="width: 70%;">
</div>

## Firmware Docs

Firmware documentation can be found [here](https://dpiclimate.github.io/ag-node)

An illustration of the devices operations can be found below:

<div align="middle"> 
	<img src="https://raw.githubusercontent.com/DPIclimate/ag-node/master/figures/flow-chart.png" style="width: 70%;">
</div>

## License

Ag-node is MIT licensed, as found in the [LICENSE](https://github.com/DPIclimate/ag-node/blob/master/LICENSE) file.
