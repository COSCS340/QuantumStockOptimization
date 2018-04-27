**Final Report**

# Quantum Stock Optimization
### Team 10
### Team Members: Joseph Huber, Thomas Huber, Michael Goin, Trevor Dixon

**Introduction**

- Working on developing an application to run an optimization problem on Quantum Computers and interpret results
- Working with:
  - Travis Humble, Director of Quantum Computing Institute at ORNL
  - A team of UTK marketing students that could potentially find a use for the code and acquire a front-end
- Have become familiar with existing code, modes, and infrastructure for executing code on DWave’s Quantum Computing Unit (QPU)
- Completed our original goals

**Customer Value**

- Customer (Travis Humble, director of Quantum Computing Institute at ORNL) hopes to leverage quantum computing for a practical optimization problem
- Original project proposal was quantum optimization of a stock portfolio 
- Due to difficulty of acquiring appropriate stock data and the preferences of the marketing team, project will now shift to cryptocurrency investment optimization
- No other changes were made

**Technology**

- Architecture: components and their relationships
  - A stock market reader fetches the data and feeds it to a Markowitz object
  - The Markowitz object handles formatting the data for output
    - Classical model to check the quantum computer’s output for testing
    - Quantum model to format the data for the quantum computer and start the job
  - XACC handles communication with the D-Wave QPU
- What works?
  - Obtained statistical financial data and completed problem statement
  - Can send a problem to the D-Wave and get output
  - Can compare classical model to D-Wave QPU output
- D-Wave solver approximates classical solver in much less time!

  
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
| Write Brute Solver   | Write a brute-force optimizer for of the problem                           | Complete    |
| Run on DWave         | Execute existing code and/or test code on DWave                            | Complete    |
| Write Solver for QPU | Write a quantum solver for the optimization problem                        | Complete    |
| Verify QPU Results   | Analyze and verify QPU results against brute solver                        | Complete    |

**Reflection**

- Accomplishments
  - Successfully sent problems to the D-Wave quantum computer
  - Scraped cryptocurrency data from the web
  - Calculated statistics from the data
  - Built QUBO Markowitz solver using cryptocurrency data
- Lessons learned
  - Don’t waste time pursuing a solution that is proving problematic; set up your own server
  - Draw diagrams for the program architecture
  - Research the problem in full-detail so you are not confused about anything
  - Use APIs that don’t have limits on usage
