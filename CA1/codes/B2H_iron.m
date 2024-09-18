function [H]=B2H_iron(B)
B_H_iron;
    [~,idx]=min(abs(B_iron-B));

    if B_iron(idx) - B  < 0
        idx2 = idx + 1;
    else 
        idx2 = idx - 1;
    end
    a = (H_iron(idx2)-H_iron(idx))/(B_iron(idx2)-B_iron(idx));
    
    H = a*(B - B_iron(idx)) + H_iron(idx);
end

