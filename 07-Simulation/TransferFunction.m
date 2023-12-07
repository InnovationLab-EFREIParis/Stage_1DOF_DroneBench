function TF_with_feedback
    % Define parameters
    c = 0.1;   % Damping coefficient
    m = 0.3;   % Mass
    g = 9.81;  % Gravitational acceleration
    dc = 0.5;  % Distance from pivot to center of mass
    d1 = 1;    % Distance from pivot to thrust point
    m_motor = 0.03; % Mass of motor
    J = 1/3*(m*d1^2) + m_motor*d1^2; % Moment of inertia
    
    % Define the linearized system matrices A and B
    A = [0, 1; -m*g*dc*0.45/J, -c/J];
    B = [0; d1/J];
    
    % Output matrix C
    C = [1, 0];
    
    % D matrix 
    D = 0;

    % Transfer function computation
    sys = ss(A, B, C, D);
    transferFunction = tf(sys);


    
    % PID Controller Parameters
    Kp = 20;  % Proportional Gain
    Ki = 5;  % Integral Gain
    Kd = 2;  % Derivative Gain

    C = pid(Kp,Ki,Kd);

    % Closed-loop transfer function with PID controller
    sys_cl_pid = feedback(sys,C);

    % Display the transfer function
    disp('Transfer Function:');
    disp(transferFunction);
    
    % Plot the step response of the system
    figure;
    
    subplot(3,1,1);
    step(transferFunction);
    title('Step Response (Open Loop)');

    subplot(3,1,2);
    impulse(sys_cl_pid);
    title('Impulse Response with PID Controller (Closed Loop)');

    subplot(3,1,3);
    step(sys_cl_pid);
    title('Impulse Response (Open Loop)');

end
