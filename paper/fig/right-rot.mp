
beginfig(1)
    u=0.2cm;
    d=10u;

    drawdot origin;
    drawdot (-1u,-2u);

    label.bot("a", (-2u,-4u));
    label.bot("b", ( 0u,-4u));
    label.bot("c", ( 1u,-2u));

    draw ( 0u, 0u)--(-1u,-2u)--(-2u,-4u);
    draw (-1u,-2u)--( 0u,-4u);
    draw ( 0u, 0u)--( 1u,-2u);


    drawdot ( 0u+d, 0u);
    drawdot ( 1u+d,-2u);

    label.bot("a", (-1u+d,-2u));
    label.bot("b", ( 0u+d,-4u));
    label.bot("c", ( 2u+d,-4u));

    draw ( 0u+d, 0u)--( 1u+d,-2u)--( 2u+d,-4u);
    draw ( 1u+d,-2u)--( 0u+d,-4u);
    draw ( 0u+d, 0u)--(-1u+d,-2u);

    drawarrow ( 4u,-2u)..(-4u+d,-2u)


endfig;
end;


