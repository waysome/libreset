beginfig(1)
    u=1cm;
    d=2u;
    for i=0 upto 7:
        draw ((0 + i)*u, 0u)--((1 + i)*u, 0u)--((1 + i)*u, 1u)--((0 + i)*u, 1u)--((0 + i)*u, 0u);
        drawarrow ((0.5 + i)*u, 0u)..((0.5 + i)*u, 1u-d);
    endfor;
    for i=0 upto 3:
        draw ((0 + i)*2u, 0u-d)--((1 + i)*2u, 0u-d)--((1 + i)*2u, 1u-d)--((0 + i)*2u, 1u-d)--((0 + i)*2u, 0u-d);
        drawarrow ((0.5 + i)*2u, 0u-d)..((0.5 + i)*2u, 1u-2d);
    endfor;
    for i=0 upto 1:
        draw ((0 + i)*4u, 0u-2d)--((1 + i)*4u, 0u-2d)--((1 + i)*4u, 1u-2d)--((0 + i)*4u, 1u-2d)--((0 + i)*4u, 0u-2d);
    endfor;
endfig;

end
