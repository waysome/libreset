beginfig(1)
    u=1cm;
    d=2u;

    % starting point
    for i=0 upto 3:
        draw ((0 + i)*u, 0u)--((1 + i)*u, 0u)--((1 + i)*u, 1u)--((0 + i)*u, 1u)--((0 + i)*u, 0u);
        label.top(substring (i,i+1) of "abcd", ((0.5 + i)*u, 0.1u));
    endfor;

    % first stage
    for i=0 upto 3:
        draw ((0 + i)*u-2d, 0u-d)--((1 + i)*u-2d, 0u-d)--((1 + i)*u-2d, 1u-d)--((0 + i)*u-2d, 1u-d)--((0 + i)*u-2d, 0u-d);
        label.top(substring (i,i+1) of "0abc", ((0.5 + i)*u-2d, 0.1u-d));
    endfor;

    % second stage
    for i=0 upto 3:
        draw ((0 + i)*u-2d, 0u-2d)--((1 + i)*u-2d, 0u-2d)--((1 + i)*u-2d, 1u-2d)--((0 + i)*u-2d, 1u-2d)--((0 + i)*u-2d, 0u-2d);
        label.top(substring (i,i+1) of "0a0c", ((0.5 + i)*u-2d, 0.1u-2d));
    endfor;

    for i=0 upto 3:
        draw ((0 + i)*u+2d, 0u-2d)--((1 + i)*u+2d, 0u-2d)--((1 + i)*u+2d, 1u-2d)--((0 + i)*u+2d, 1u-2d)--((0 + i)*u+2d, 0u-2d);
        label.top(substring (i,i+1) of "0b0d", ((0.5 + i)*u+2d, 0.1u-2d));
    endfor;

    % result
    for i=0 upto 1:
        draw ((0 + i)*2u, 0u-3d)--((1 + i)*2u, 0u-3d)--((1 + i)*2u, 1u-3d)--((0 + i)*2u, 1u-3d)--((0 + i)*2u, 0u-3d);
        label.top(substring (3*i,3*i+3) of "a+bc+d", ((0.5 + i)*2u, 0.1u-3d));
    endfor;

    % start -> stages
    drawarrow (0u, 0.5u){left}..(2u-2d, 1u-d){down};
    drawarrow (4u, 0.5u){right}..(2u+2d, 1u-2d){down};

    % first stage -> second stage
    drawarrow (2u-2d, 0u-d){down}..(2u-2d, 1u-2d){down};

    % second stage -> result
    drawarrow (2u-2d, 0u-2d){down}..(0u, 0.5u-3d){right};
    drawarrow (2u+2d, 0u-2d){down}..(4u, 0.5u-3d){left};

    %labels
    label.top("shift", (-2u, 0u));
    label.top("mask", (2u, -1u-d));
    label.top("add", (2u, -1u-2d));

endfig;

end

