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
    
    poles = [-4.1, -4.2];
    K = place(A, B, poles);

    % Define the closed-loop system with feedback
    A_cl = A - B*K;
    B_cl = B;
    C_cl = C;
    D_cl = D;

    % Create the closed-loop state-space system
    sys_cl = ss(A_cl, B_cl, C_cl, D_cl);

    % Display the transfer function
    disp('Transfer Function:');
    disp(transferFunction);
    
    % Plot the step response of the system
    figure;
    
    subplot(3,1,1);
    step(transferFunction);
    title('Reponse indicielle (Open Loop)');

    subplot(3,1,2);
    step(sys_cl);
    title('Reponse indicielle avec retroaction (Closed Loop)');

    subplot(3,1,3);
    impulse(transferFunction);
    title('Reponse a impulsion de Dirac (Open Loop)');

end
