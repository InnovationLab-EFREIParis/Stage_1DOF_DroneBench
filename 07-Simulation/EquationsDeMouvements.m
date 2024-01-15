function pendulumSolver
    % Define parameters
    c = 0.1;   % Damping coefficient
    m = 0.3;     % Mass
    g = 9.81;  % Gravitational acceleration
    dc = 0.5;    % Distance from pivot to center of mass
    d1 = 1;    % Distance from pivot to thrust point
    m_motor = 0.03; % Mass of motor
    J = 1/3*(m*d1^2) + m_motor*d1^2;     % Moment of inertia
    
    % Thrust
    T = @(t) 2;
    
    % Define the differential equation
    odefun = @(t, y) pendulumODE(t, y, J, c, m, g, dc, d1, T);
    
    % Set initial conditions in degrees
    initial_theta = 0;         % Initial angle (in degrees)
    initial_theta_dot = 0;      % Initial angular velocity
    
    initial_conditions = [initial_theta; initial_theta_dot];
    
    % Set the time span
    tspan = [0 400];  % Time span for simulation
    
    % Solve the ODE
    [t, y] = ode45(odefun, tspan, deg2rad(initial_conditions));
    
    % Convert results to degrees and ensure angles are within the range [0, 90]
    y(:,1) = rad2deg(y(:,1));

    
    % Plot the results
    figure;
    subplot(2,1,1);
    plot(t, y(:,1));
    title('Pendulum Angle vs Time');
    xlabel('Time (s)');
    ylabel('Angle (deg)');
    
    subplot(2,1,2);
    plot(t, y(:,2));
    title('Angular Velocity vs Time');
    xlabel('Time (s)');
    ylabel('Angular Velocity (rad/s)');
    
end

function dydt = pendulumODE(t, y, J, c, m, g, dc, d1, T)
    % ODE function for the pendulum with thrust
    theta = y(1);
    theta_dot = y(2);
    
    % External torque (thrust)
    torque_thrust = d1 * T(t);
    
    % Equation of motion
    dydt = zeros(2, 1);
    dydt(1) = theta_dot;
    dydt(2) = (torque_thrust - m * g * dc * sin(theta) - c * theta_dot ) / J;
end
