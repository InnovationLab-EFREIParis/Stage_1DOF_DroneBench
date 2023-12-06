function linearPendulumSolver
    % Define parameters
    c = 0.1;   % Damping coefficient
    m = 0.3;   % Mass
    g = 9.81;  % Gravitational acceleration
    dc = 0.5;  % Distance from pivot to center of mass
    d1 = 1;    % Distance from pivot to thrust point
    m_motor = 0.03; % Mass of motor
    J = 1/3*(m*d1^2) + m_motor*d1^2;     % Moment of inertia
    
    % Thrust
    T = @(t) 1;
    
    % Define the linearized system matrices A and B
    A = [0, 1; -m*g*dc/J, -c/J];
    B = [0; d1/J];

    % Compute eigenvalues of matrix A
    eigenvalues_A = eig(A);
    
    % Display the eigenvalues
    disp('Eigenvalues of matrix A:');
    disp(eigenvalues_A);
    
    % Set initial conditions in degrees
    initial_theta = 0;         % Initial angle (in degrees)
    initial_theta_dot = 0;      % Initial angular velocity
    
    initial_conditions = [deg2rad(initial_theta); deg2rad(initial_theta_dot)];
    
    % Set the time span
    tspan = [0 10];  % Time span for simulation
    
    % Solve the linearized system
    [t, x] = ode45(@(t, x) linearPendulumODE(t, x, A, B, T), tspan, initial_conditions);
    
    % Convert results to degrees
    x(:,1) = rad2deg(x(:,1));

    
    % Plot the results
    figure;
    subplot(2,1,1);
    plot(t, x(:,1));
    title('Pendulum Angle vs Time (Linearized)');
    xlabel('Time (s)');
    ylabel('Angle (deg)');
    
    subplot(2,1,2);
    plot(t, x(:,2));
    title('Angular Velocity vs Time (Linearized)');
    xlabel('Time (s)');
    ylabel('Angular Velocity (rad/s)');

    
end

function dxdt = linearPendulumODE(t, x, A, B, T)
    % Linearized ODE function for the pendulum with thrust
    u = T(t);
    dxdt = A * x + B * u;
end
