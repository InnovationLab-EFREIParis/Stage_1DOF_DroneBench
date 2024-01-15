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
    A = [0, 1; -m*g*dc/J, -c/J];
    B = [0; d1/J];
    
    % Output matrix C
    C = [1, 0];
    
    % D matrix 
    D = 0;

    % Transfer function computation
    sys = ss(A, B, C, D);
    transferFunction = tf(sys);

    %numerator = [1];
    %denominator = [J c m*g*dc];

    % disp(tf(numerator,denominator));

    % impulse(tf(numerator,denominator));
    
    bode(transferFunction);
    % PID Controller Parameters
    Kp = 100;  % Proportional Gain
    Ki = 5;  % Integral Gain
    Kd = 2;  % Derivative Gain

    C = pid(Kp,Ki,Kd);

    % Closed-loop transfer function with PID controller
    sys_cl_pid = feedback(sys*C,1);
    bode(sys_cl_pid);
    % Plot Closed-Loop Step Response with PID controller
    figure;
    subplot(2,1,1);
    step(transferFunction, '-');
    title('Fonction de transfer step');
    

    % Plot Closed-Loop Impulse Response with PID controller
    subplot(2,1,2);
    impulse(transferFunction, '-');
    title('Fonction de transfer impulse');

    % Plot Closed-Loop Step Response with LQR controller
    figure;
    subplot(2,1,1);
    step(sys_cl_pid, '-');
    title('Boucle fermee PID step');

    % Plot Closed-Loop Impulse Response with LQR controller
    subplot(2,1,2);
    impulse(sys_cl_pid, '-');
    title('Boucle fermee PID impulse');
   
end
