**Status Report**

# Quantum Financial Optimization
### Team 10
### Team Members: Joseph Huber, Thomas Huber, Michael Goin, Trevor Dixon

**Introduction**

- Working on developing an application to run an optimization problem on Quantum Computers and interpret results
- Working with:
  - Travis Humble, Director of Quantum Computing Institute at ORNL
  - A team of UTK marketing students that could potentially find a use for the code and acquire a front-end
- Have become familiar with existing code, modes, and infrastructure for executing code on DWave’s Quantum Computing Unit (QPU)

**Customer Value**

- Customer (Travis Humble, director of Quantum Computing Institute at ORNL) hopes to leverage quantum computing for a practical optimization problem
- Original project proposal was quantum optimization of a stock portfolio 
- Due to difficulty of acquiring appropriate stock data and the preferences of the marketing team, project will now shift to cryptocurrency investment optimization

**Technology**

- Architecture: components and their relationships
  - A stock market reader fetches the data and feeds it to a Markowitz object
  - The Markowitz object handles formatting the data for output
    - Classical model to check the quantum computer’s output for testing
    - Quantum model to format the data for the quantum computer and start the job
  - XACC handles communication with the D-Wave QPU
- Goals
  - Send a problem to the D-Wave quantum computer
  - Complete a classical model to cross-check the quantum computer’s output
- What works?
  - Can send a problem to the D-Wave and get output
- Minimal testing, still developing the problem and checking the output
- Next iteration goals
  - Obtain statistical financial data and complete problem statement
  - Refine software architecture
  
**Team Roles**

- Joseph Huber
  - Major: Computer Engineering
  - Role: Team Leader
- Thomas Huber
  - Major: Computer Engineering
  - Role: Problem Research
- Michael Goin
  - Major: Computer Science
  - Role: Project Management
- Trevor Dixon
  - Major: Computer Science
  - Role: Documentation

**Project Management**

| Task                 | Description                                                                | Status      |
|----------------------|----------------------------------------------------------------------------|-------------|
| Project Introduction | Conference Call and Initial Meeting with Travis (QCI Director)             | Complete    |
| Repo Configuration   | Setup repo and all get access                                              | Complete    |
| XACC Build/DWave     | Successfully build XACC and get DWave access                               | Complete    |
| Markowitz            | Read and familiarize with Markowitz model and existing code from the group | Complete    |
| Acquire Data         | Acquire cryptocurrency valuation data through time                         | Complete    |
| Write Brute Solver   | Write a brute-force optimizer for of the problem                           | In Progress |
| Run on DWave         | Execute existing code and/or test code on DWave                            | In Progress |
| Write Solver for QPU | Write a quantum solver for the optimization problem                        | Pending     |
| Verify QPU Results   | Analyze and verify QPU results against brute solver                        | Pending     |

**Reflection**

- Problems for this iteration
  - Setting up a server that can connect to the D-Wave computer consumed a lot of time
  - Getting libraries to link properly
  - Formulating problem and program architecture
- Accomplishments
  - Set up a functioning Docker container
  - Successfully sent a problem to the D-Wave quantum computer
- Lessons learned
  - Don’t waste time pursuing a solution that is proving problematic; set up your own server
  - Draw diagrams for the program architecture
  - Research the problem

