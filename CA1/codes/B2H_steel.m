function [H]=B2H_steel(B)

B_H_steel;

[~,idx]=min(abs(B_steel-B));

    if B_steel(idx) - B  < 0
        idx2 = idx + 1;
    else 
        idx2 = idx - 1;
    end
    a = (H_steel(idx2)-H_steel(idx))/(B_steel(idx2)-B_steel(idx));
    
    H = a*(B - B_steel(idx)) + H_steel(idx);
end

