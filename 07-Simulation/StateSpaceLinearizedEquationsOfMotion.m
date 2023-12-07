function computeAndPlotOutput
    % Parameters
    c = 0.1;   % Damping coefficient
    m = 0.3;   % Mass
    g = 9.81;  % Gravitational acceleration
    dc = 0.5;  % Distance from pivot to center of mass
    d1 = 1;    % Distance from pivot to thrust point
    m_motor = 0.03; % Mass of motor
    J = 1/3*(m*d1^2) + m_motor*d1^2; % Moment of inertia
    
    % Define the linearized system matrices A and B
    A = [0, 1; (-m*g*dc)/J, -c/J];
    B = [0; d1/J];
    
    % Output matrix C
    C = [1, 0];
    
    % D matrix
    D = 0;

    % Set initial conditions in degrees
    initial_theta = 0;         % Initial angle (in degrees)
    initial_theta_dot = 0;      % Initial angular velocity
    
    initial_conditions = [deg2rad(initial_theta); deg2rad(initial_theta_dot)];

    % Time span for simulation
    tspan = [0 10];

    % Input function (thrust torque)
    u = @(t) 1*d1;

    % Simulate the linearized system
    [t, x] = ode45(@(t, x) linearPendulumODE(t, x, A, B, u), tspan, initial_conditions);
    x(:,1) = rad2deg(x(:,1));

    % Compute the output y = Cx + Du
    y = (C * x' + D * u(t)').';
    
    disp("Eigenvalues of A :");
    disp(eig(A));

    disp("Check if rank Ctrb(A,B) has full rank : ");
    CtrbMatrice = ctrb(A,B);
    disp(rank(CtrbMatrice));

    % Plot the output
    figure;
    plot(t, y, 'LineWidth', 1);
    title('Output of Linearized System (Output: Angle)');
    xlabel('Time (s)');
    ylabel('Output (Angle)');
end

function dxdt = linearPendulumODE(t, x, A, B, u)
    % Linearized ODE function for the pendulum with thrust
    dxdt = A * x + B * u(t);
end
