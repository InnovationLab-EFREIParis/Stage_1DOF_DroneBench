function simulateControlledPendulum
    % Define parameters
    c = 0.1;   % Damping coefficient
    m = 0.3;   % Mass
    g = 9.81;  % Gravitational acceleration
    dc = 0.5;  % Distance from pivot to center of mass
    d1 = 1;    % Distance from pivot to thrust point
    m_motor = 0.03; % Mass of motor
    J = 1/3*(m*d1^2) + m_motor*d1^2; % Moment of inertia
    
    % Linearized system matrices A and B
    A = [0, 1; -m*g*dc/J, -c/J];
    B = [0; d1/J];

    % Equilibrium point
    equilibrium_angle = 0.45;

    % Controller gains
    poles = [-50.2, -50.3];
    K = place(A, B, poles);

    disp(eig(A-B*K));

    % Define the control law
    control_law = @(t, x) -K * (x - [equilibrium_angle; 0]) + 3*d1; % Proportional control + constant thrust

    % Set initial conditions
    initial_theta = 0;
    initial_theta_dot = 0;
    initial_conditions = [deg2rad(initial_theta); deg2rad(initial_theta_dot)];

    % Set the time span for simulation
    tspan = [0 10];

    % Simulate the closed-loop system with the control law
    [t, y] = ode45(@(t, x) controlledPendulumODE(t, x, A, B, control_law), tspan, initial_conditions);

    % Convert results to degrees
    y(:, 1) = rad2deg(y(:, 1));

    % Plot the results
    figure;
    subplot(2, 1, 1);
    plot(t, y(:, 1), 'LineWidth', 2);
    title('Pendulum Angle vs Time (with Proportional Control and Thrust)');
    xlabel('Time (s)');
    ylabel('Angle (deg)');

    subplot(2, 1, 2);
    plot(t, y(:, 2), 'LineWidth', 2);
    title('Angular Velocity vs Time (with Proportional Control and Thrust)');
    xlabel('Time (s)');
    ylabel('Angular Velocity (rad/s)');
end

function dxdt = controlledPendulumODE(t, x, A, B, control_law)
    % Controlled ODE function for the pendulum
    u = control_law(t, x);
    dxdt = A * x + B * u;
end
