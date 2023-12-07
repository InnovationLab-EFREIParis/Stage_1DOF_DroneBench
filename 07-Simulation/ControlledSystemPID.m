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
    equilibrium_angle = 0.12;

    % Proportional, Integral, and Derivative gains for the PID controller
    Kp = 20;
    Ki = 5;  
    Kd = 2;  

    % Define initial values for integral and derivative terms
    integral_term = 0;
    prev_error = 0;

    % Define the control law
    control_law = @(t, x) -Kp * (x(1) - equilibrium_angle) - Ki * integral_term - Kd * (x(2) - prev_error) + 3*d1;

    % Set initial conditions
    initial_theta = 0;
    initial_theta_dot = 0;
    initial_conditions = [deg2rad(initial_theta); deg2rad(initial_theta_dot)];

    % Set the time span for simulation
    tspan = [0 10];

    % Simulate the closed-loop system with the PID controller
    [t, y] = ode45(@(t, x) controlledPendulumODE(t, x, A, B, control_law), tspan, initial_conditions);

    % Inside the loop where you integrate the system, update integral and derivative terms
    for i = 2:length(t)
        dt = t(i) - t(i-1);
        error = y(i, 1) - equilibrium_angle;

        % Update integral and derivative terms
        integral_term = integral_term + error * dt;
        derivative_term = (error - prev_error) / dt;

        % Update the control law with the updated terms
        control_law = @(t, x) -Kp * error - Ki * integral_term - Kd * derivative_term + 3*d1;

        % Update previous error for the next iteration
        prev_error = error;
    end

    % Convert results to degrees
    y(:,1) = rad2deg(y(:,1));

    % Plot the results
    figure;
    subplot(2,1,1);
    plot(t, y(:,1), 'LineWidth', 2);
    title('Pendulum Angle vs Time (with PID Control and Thrust)');
    xlabel('Time (s)');
    ylabel('Angle (deg)');

    subplot(2,1,2);
    plot(t, y(:,2), 'LineWidth', 2);
    title('Angular Velocity vs Time (with PID Control and Thrust)');
    xlabel('Time (s)');
    ylabel('Angular Velocity (rad/s)');
end

function dxdt = controlledPendulumODE(t, x, A, B, control_law)
    % Controlled ODE function for the pendulum
    u = control_law(t, x);
    dxdt = A * x + B * u;
end
