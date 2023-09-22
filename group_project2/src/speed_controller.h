
class PController
{
    public:
        PController(double k);
        double update(double ref , double actual);

    private:
        double kp;
};