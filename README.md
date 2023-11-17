# TCP Tahoe Congestion Control Mini Project
Simulation of TCP Congestion Window increase of Tahoe using socket programming in C

## Overview

This repository contains the source code and documentation for simulating the TCP Tahoe Congestion Control algorithm using socket programming in the C language. The project consists of server and client programs that communicate over a simulated network, implementing key aspects of TCP Tahoe's congestion control mechanism.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Simulation Details](#simulation-details)
- [Contributing](#contributing)

## Features

- TCP Tahoe Congestion Control Simulation
- Server and Client Implementation
- Socket Programming in C
- Realistic Data Transmission and Acknowledgment Handling
- Graphical Output of Congestion Window Evolution
- Easy-to-Understand Code Structure

## Prerequisites

- C Compiler (eg.GCC.,)
- XGraph for plotting

## Getting started

1. Compile the server and client programs in terminal:
   
    gcc server.c -o server
    gcc client.c -o client
   
3. Run the server and client on separate terminals:

    - Terminal 1 (Server):
        ./server <port_number>

    - Terminal 2 (Client):
        ./client <port_number>
        
4. Run output.xg file using XGraph
   
     - ./xgraph output.xg
     - 
## Usage

- Follow the prompts in the client terminal to simulate the transmission of packets and observe the congestion window evolution.
- View the graphical output (optional) generated in the `output.xg` file.

## Simulation Details

- The simulation mimics the TCP Tahoe Congestion Control algorithm, including slow start, congestion avoidance, and fast retransmit.
- Server and client programs communicate over a simulated network, adjusting the congestion window size dynamically.
