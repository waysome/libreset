u=1cm;

m=64;

zeta=1 - 1/m;


vardef num(expr lev) =
    (2**(lev)-1)
enddef;


beginfig(1)
    xd:=u;
    yd:=2u;

    %draw axis
    drawarrow (0xd, 0yd)..(7xd, 0yd);
    drawarrow (0xd, 0yd)..(0xd, 2.5yd);

    for x=1 upto 6:
        draw (x*xd, 0yd)--(x*xd, 0yd-2mm);
        label.bot(decimal x, (x*xd, 0yd-2mm));
    endfor;

    for y=0 upto 2:
        draw (0xd, y*yd)--(0xd-2mm, y*yd);
        label.lft(decimal y, (0xd-2mm, y*yd));
    endfor;

    label.bot("l", (7xd, 0yd));
    label.top(btex $T_{\in}$ etex, (0xd, 2.5yd));

    for k=2 upto 4:
        %draw T_{\in}
        draw (0xd, 0yd)
        for l=1 upto 7:
            ..(l*xd, (0
            for i=0 upto l-1:
                +(1
                for j=0 upto i:
                    *((1 - zeta**(k*num(l-j)))**k)
                endfor
                )
            endfor
            )*yd)
        endfor withcolor (k/2-1)[red,blue];
        label.top("k="&decimal k, (6xd, 0yd+(20-5*k)*mm))
            withcolor (k/2-1)[red,blue];
    endfor;

    %draw id for comparision
    draw (0xd, 0yd)--(2.5xd, 2.5yd);
    label.top("id", (2.5xd, 2.5yd));
endfig;


beginfig(2)
    xd:=u;
    yd:=0.005*u;

    %draw axis
    drawarrow (0xd, 0yd)..(7xd, 0yd);
    drawarrow (0xd, -500yd)..(0xd, 1000yd);

    for x=1 upto 6:
        draw (x*xd, 0yd)--(x*xd, 0yd-2mm);
        label.bot(decimal x, (x*xd, 0yd-2mm));
    endfor;

    for n=-2 upto 5:
        draw (0xd, mlog(2**n)*yd)--(0xd-2mm, mlog(2**n)*yd);
        label.lft(decimal (2**n), (0xd-2mm, mlog(2**n)*yd));
    endfor;

    path clipp;
    clipp := (0xd, -500yd)--(7xd, -500yd)--(7xd, 1000yd)--(0xd, 1000yd)--cycle;

    label.bot("l", (7xd, 0yd));
    label.top(btex $T_{\cap}$ etex, (0xd, 1000yd));

    for k=2 upto 4:
        %draw T_{\cap}
        picture graph;
        graph := image(
            draw (0xd, -infinity){up}
            for l=1 upto 6:
                ..(l*xd, mlog(0
                for i=0 upto l-1:
                    +((2**i)
                    for j=0 upto i:
                        *(1 - (
                            zeta**(2*k*num(l-j))
                            -2*(zeta**(k*num(l-j)))
                        )**(m-k+1))
                    endfor
                    )
                endfor
                )*yd)
            endfor withcolor (k/2-1)[red,blue];
        );
        clip graph to clipp;
        draw graph;
        label.top("k="&decimal k, (6xd, 0yd+(20-5*k)*mm))
            withcolor (k/2-1)[red,blue];
    endfor;

    %draw id for comparision
    picture id;
    id := image(
        draw (0.1xd, mlog(num(0.1))*yd)
        for l=1 upto 6:
            ..(l*xd, mlog(num(l))*yd)
        endfor;
    );
    clip id to clipp;
    draw id;
    label.top("n(l)", (6xd, 1000yd));
endfig;
end;


